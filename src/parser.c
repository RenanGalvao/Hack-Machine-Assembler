#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "hash-map.h"
#include "code.h"
#include "defs.h"
#include "utils.h"

// first scan - get labels
void first_scan(FILE *source_file) {
    extern struct HashMap *symbols_table;

    const char re_is_label[] = "\\(.+\\)";
    regmatch_t pmatch[1];

    char line[MAX_LINE_LENGHT];
    int line_number = 0;
    int label_count = 0; // compesate for labels

    while(fgets(line, MAX_LINE_LENGHT, source_file) != NULL) {
        // prepare line
        clear_line(line);

        // ignore empty lines
        if(is_empty_string(line)) {
            continue;
        }

        // process
        re_match(re_is_label, line, pmatch);
        int off = pmatch[0].rm_so;
        int len = pmatch[0].rm_eo - pmatch[0].rm_so;

        if(off != -1) {
            char label[MAX_SYMBOL_LENGTH];
            char binary[WORD_SIZE + 1];

            // strip '(' and ')'
            snprintf(label, MAX_SYMBOL_LENGTH, "%.*s", len - 2, line + 1);

            //decimal_to_binary(line_number - label_count, binary, WORD_SIZE + 1, 1);
            symbols_table->put(symbols_table, label, line_number - label_count);
            label_count++;
        }

        line_number++;
    }

    rewind(source_file);
}

// second scan - translate instructions and find errors
// ignore label instructions (xxx), just A and C get translated
int second_scan(FILE *source_file, FILE *output_file) {
    extern struct HashMap *comp_table;
    extern struct HashMap *dest_table;
    extern struct HashMap *jump_table;

    const char re_is_label[] = "\\(.+\\)";
    const char re_is_a_instruction[] = "@";
    const char re_is_c_instruction[] = "[;=]";
    regmatch_t pmatch[1];

    char line[MAX_LINE_LENGHT];
    int line_number = 0;

    while(fgets(line, MAX_LINE_LENGHT, source_file) != NULL) {
        // prepare line
        clear_line(line);

        // ignore empty lines
        if(is_empty_string(line)) {
            line_number++;
            continue;
        }

        // A Instruction
        re_match(re_is_a_instruction, line, pmatch);
        if(pmatch[0].rm_so != -1) {
            // strip @
            char instruction[MAX_SYMBOL_LENGTH];
            snprintf(instruction, MAX_SYMBOL_LENGTH, "%s", line + 1);

            if(is_empty_string(instruction)) {
                bad_instruction(line, line_number);
                return -1;
            }

            if(atoi(instruction) < 0 || atoi(instruction) > MAX_A_INSTRUCT_CONSTANT) {
                bad_instruction(line, line_number);
                return -1;
            }

            write_a_instruction(instruction, output_file);

            // next instruction
            line_number++;
            continue;
        }

        // C Instruction
        re_match(re_is_c_instruction, line, pmatch);
        if(pmatch[0].rm_so != -1) {
            int16_t table_value = -1;

            // jump
            if(strchr(line, ';')) {
                char **parts = explode(line, ";");
                int size = arrlen(parts);

                if(size != 2) {
                    bad_instruction(line, line_number);
                    return -1;
                }

                // first part of instruction - computation
                table_value = comp_table->get(comp_table, parts[0]);
                if(table_value == -1) {
                    bad_instruction(line, line_number);
                    return -1;
                }

                // second part - jump test
                table_value = jump_table->get(jump_table, parts[1]);
                if(table_value == -1) {
                    bad_instruction(line, line_number);
                    return -1;
                }

                free_exploded(parts);
                write_c_instruction(line, output_file);
            }

            // assignment
            else {
                char **parts = explode(line, "=");
                int size = arrlen(parts);

                if(size != 2) {
                    bad_instruction(line, line_number);
                    return -1;
                }

                // first part of instruction - destination
                table_value = dest_table->get(dest_table, parts[0]);
                if(table_value == -1) {
                    bad_instruction(line, line_number);
                    return -1;
                }

                // second part - computation
                // memory computation a = 1
                table_value = comp_table->get(comp_table, parts[1]);
                if(table_value == -1) {
                    bad_instruction(line, line_number);
                    return -1;
                }
                free_exploded(parts);
                write_c_instruction(line, output_file);
            }

            // next instruction
            line_number++;
            continue;
        }

        // Label
        re_match(re_is_label, line, pmatch);
        if(pmatch[0].rm_so != -1) {

            // ignore
            line_number++;
            continue;
        }

        // invalid instruction
        bad_instruction(line, line_number);
        return -1;
    }

    return 0;
}

void bad_instruction(const char *line, int line_number) {
    fprintf(stderr, "Bad instruction at line %d: '%s'\n", line_number+1, line);
}

