#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code.h"
#include "defs.h"
#include "hash-map.h"
#include "utils.h"

void write_a_instruction(char *str_instruction, FILE *output_file) {
    extern struct HashMap *symbols_table;

    static int next_address = 16;
    char binary[WORD_SIZE + 1];
    int16_t table_value = symbols_table->get(symbols_table, str_instruction);

    if(table_value == -1) {
        // used as data
        if(is_numeric_string(str_instruction)) {
            decimal_to_binary(atoi(str_instruction), binary, WORD_SIZE + 1, 1);
        }
        // new var
        else {
            decimal_to_binary(next_address, binary, WORD_SIZE + 1, 1);
            symbols_table->put(symbols_table, str_instruction, next_address++);
        }
    } else {
        decimal_to_binary(table_value, binary, WORD_SIZE + 1, 1);
    }

    write_output(binary, output_file);
}

void _write_jump_instruction(char *str_instruction, FILE *output_file) {
    extern struct HashMap *comp_table;
    extern struct HashMap *jump_table;

    // a + comp + dest + jump bits = 13
    // first 3 bits set to 1, '111' = 7
    uint16_t instruction = (uint16_t)(7 << 13);

    // comp + dest + jump bits = 12
    const uint16_t a_bit_mask = 1 << 12;

    char **parts = explode(str_instruction, ";");

    // first part of instruction - computation
    // a bit
    if(strchr(parts[0], 'M')) {
        instruction ^= a_bit_mask;
    }
    // rest of comp
    uint16_t table_value = comp_table->get(comp_table, parts[0]);
    instruction ^= table_value;

    // second part - jump test
    table_value = jump_table->get(jump_table, parts[1]);
    instruction ^= table_value;

    char binary[WORD_SIZE + 1];
    decimal_to_binary(instruction, binary, WORD_SIZE + 1, 1);

    write_output(binary, output_file);
    free_exploded(parts);
}

void _write_assignment_instruction(char *str_instruction, FILE *output_file) {
    extern struct HashMap *comp_table;
    extern struct HashMap *dest_table;

    // see comments in _write_jump_instruction
    uint16_t instruction = (uint16_t)(7 << 13);

    const uint16_t a_bit_mask = 1 << 12;

    // set bits for destination
    // M = [2]; D = [1]; A = [0]
    // dest[2] = strchr(token, 'M') ? '1' : '0'; 
    // dest[1] = strchr(token, 'D') ? '1' : '0'; 
    // dest[0] = strchr(token, 'A') ? '1' : '0'; 

    char **parts = explode(str_instruction, "=");
    // first part of instruction - destination
    uint16_t table_value = dest_table->get(dest_table, parts[0]);
    instruction ^= table_value;

    // second part - computation
    // a bit
    if(strchr(parts[1], 'M')) {
        instruction ^= a_bit_mask;
    }
    // rest of comp
    table_value = comp_table->get(comp_table, parts[1]);
    instruction ^= table_value;

    char binary[WORD_SIZE + 1];
    decimal_to_binary(instruction, binary, WORD_SIZE + 1, 1);
    write_output(binary, output_file);
    free_exploded(parts);
}

void write_c_instruction(char *str_instruction, FILE *output_file) {
    if(strchr(str_instruction, ';')) {
        _write_jump_instruction(str_instruction, output_file);
    } else {
        _write_assignment_instruction(str_instruction, output_file);
    }
}
