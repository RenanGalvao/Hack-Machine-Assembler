#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "regex.h"
#include "utils.h"
#include "tables.h"

// first scan - get labels
void first_scan(FILE *source_file, struct HashMap *symbols_table) {
    char re_is_label[] = "\\(.+\\)";
    regmatch_t pmatch[1];

    int line_size = 256;
    char line[line_size];
    int line_number = 0;
    int label_count = 0; // compesate for labels

    while(fgets(line, line_size, source_file) != NULL) {
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

        #ifdef DEBUG
        printf("[parser.c] original string: '%s'; off: %d; length: %d; line:%d;\n", line, off, len, line_number);
        #endif // DEBUG

        if(off != -1) {
            char tmp[16];
            char binary[17];

            // strip '(' and ')'
            snprintf(tmp, 16, "%.*s", len - 2, line + 1);

            decimal_to_binary(line_number - label_count, binary, 17, 1);
            symbols_table->put(symbols_table, tmp, binary);
            label_count++;

            #ifdef DEBUG
            printf("[parser.c] label: '%s'; address: '%s'\n", tmp, binary);
            #endif // DEBUG
        }

        line_number++;
    }

    rewind(source_file);
}

// second scan - translate instructions and find errors
// ignore label instructions (xxx), just A and C get translated
int second_scan(FILE *source_file, struct HashMap *symbols_table, FILE *output_file) {
    char re_is_label[] = "\\(.+\\)";
    char re_is_a_instruction[] = "@";
    char re_is_c_instruction[] = "[;=]";
    regmatch_t pmatch[1];

    struct HashMap *comp_table = get_comp_table();
    struct HashMap *dest_table = get_dest_table();
    struct HashMap *jump_table = get_jump_table();

    int line_size = 256;
    char line[line_size];
    int line_number = 0;
    int next_address = 16;

    while(fgets(line, line_size, source_file) != NULL) {
        char binary[17];
        char *table_value = NULL;

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
            char tmp[16];
            snprintf(tmp, 16, "%s", line + 1);

            if(strlen(tmp) == 0) {
                bad_instruction(line, line_number);
                clean_up(comp_table, dest_table, jump_table);

                return -1;
            }

            table_value = symbols_table->get(symbols_table, tmp);
            if(!table_value) {
                // used as data
                if(is_numeric_string(tmp)) {
                    decimal_to_binary(atoi(tmp), binary, 17, 1);
                }
                // new var
                else {
                    decimal_to_binary(next_address++, binary, 17, 1);
                    symbols_table->put(symbols_table, tmp, binary);
                }
            } else {
                strcpy(binary, table_value);
            }

            #ifdef DEBUG
            printf("[parser.c] instruction #%d; original string: '%s'; binary value: '%s';\n", line_number, line, binary);
            #endif // DEBUG

            write_output(binary, output_file);

            // next instruction
            line_number++;
            continue;
        }

        // C Instruction
        char original[16];
        strcpy(original, line);

        re_match(re_is_c_instruction, line, pmatch);
        if(pmatch[0].rm_so != -1) {
            char dest[4];
            char comp[8]; // 7 with 'a' bit
            char jump[4];

            decimal_to_binary(0, dest, 4, 1);
            decimal_to_binary(0, comp, 8, 1);
            decimal_to_binary(0, jump, 4, 1);

            // jump
            if(strchr(line, ';')) {
                char *token = strtok(line, ";");

                if(token == NULL) {
                    bad_instruction(original, line_number);
                    clean_up(comp_table, dest_table, jump_table);

                    return -1;
                }

                // memory computation a = 1
                comp[0] = strchr(token, 'M') ? '1' : '0'; 

                // first part of instruction - computation
                table_value = comp_table->get(comp_table, token);
                if(!table_value) {
                    bad_instruction(original, line_number);
                    clean_up(comp_table, dest_table, jump_table);

                    return -1;
                }
                strcpy(comp+1, table_value);

                // second part - jump test
                token = strtok(NULL, ";");
                table_value = jump_table->get(jump_table, token);
                if(!table_value) {
                    bad_instruction(original, line_number);
                    clean_up(comp_table, dest_table, jump_table);

                    return -1;
                }
                strcpy(jump, table_value);

                #ifdef DEBUG
                printf("[parser.c] instruction #%d; original string: '%s'; comp: %s; dest: %s; jump:%s;\n", line_number, original, comp, dest, jump);
                #endif // DEBUG

                snprintf(binary, 17, "111%s%s%s", comp, dest, jump);
                write_output(binary, output_file);
            }

            // assignment
            else {
                char *token = strtok(line, "=");
                if(token == NULL) {
                    bad_instruction(original, line_number);
                    clean_up(comp_table, dest_table, jump_table);

                    return -1;
                }

                // set bits for destination
                // M = [2]; D = [1]; A = [0]
                // dest[2] = strchr(token, 'M') ? '1' : '0'; 
                // dest[1] = strchr(token, 'D') ? '1' : '0'; 
                // dest[0] = strchr(token, 'A') ? '1' : '0'; 

                // first part of instruction - destination
                table_value = dest_table->get(dest_table, token);
                if(!table_value) {
                    bad_instruction(original, line_number);
                    clean_up(comp_table, dest_table, jump_table);

                    return -1;
                }
                strcpy(dest, table_value);

                // second part - computation
                token = strtok(NULL, "=");
                if(token == NULL) {
                    bad_instruction(original, line_number);
                    clean_up(comp_table, dest_table, jump_table);

                    return -1;
                }
                // memory computation a = 1
                comp[0] = strchr(token, 'M') ? '1' : '0';
                table_value = comp_table->get(comp_table, token);
                if(!table_value) {
                    bad_instruction(original, line_number);
                    clean_up(comp_table, dest_table, jump_table);

                    return -1;
                }
                strcpy(comp+1,  table_value);

                #ifdef DEBUG
                printf("[parser.c] instruction #%d; original string: '%s'; comp: %s; dest: %s; jump:%s;\n", line_number, original, comp, dest, jump);
                #endif // DEBUG

                snprintf(binary, 17, "111%s%s%s", comp, dest, jump);
                write_output(binary, output_file);
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
        clean_up(comp_table, dest_table, jump_table);

        return -1;
    }

    clean_up(comp_table, dest_table, jump_table);
    return 0;
}

void bad_instruction(const char *line, int line_number) {
    fprintf(stderr, "Bad instruction at line %d: '%s'\n", line_number+1, line);
}

void clean_up(struct HashMap *comp_table, struct HashMap *dest_table, struct HashMap *jump_table) {
    comp_table->del(comp_table);
    dest_table->del(dest_table);
    jump_table->del(jump_table);
}
