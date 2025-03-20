#include "code.h"
#include "defs.h"
#include "tables.h"
#include "utils.h"
#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

// force hash-map.h
TEST_SOURCE_FILE("hash-map.c");

struct HashMap *symbols_table = NULL;
struct HashMap *comp_table = NULL;
struct HashMap *dest_table = NULL;
struct HashMap *jump_table = NULL;

const char output_file_path[] = "test/files/write_instruction.hack";
FILE *output_file = NULL;
int fd_output_file = 0;

const int TEST_MAX_LINE_LENGHT = MAX_LINE_LENGHT * 8;
char *line;

void setUp(void) {
    symbols_table = get_symbols_table();
    comp_table = get_comp_table();
    dest_table = get_dest_table();
    jump_table = get_jump_table();

    output_file = fopen(output_file_path, "w");
    fd_output_file = open(output_file_path, O_RDONLY);
    line = malloc(sizeof(char) * TEST_MAX_LINE_LENGHT);
}

void tearDown(void) {
    symbols_table->del(symbols_table);
    comp_table->del(comp_table);
    dest_table->del(dest_table);
    jump_table->del(jump_table);

    fclose(output_file);
    close(fd_output_file);
    remove(output_file_path);
    free(line);
}

void test_write_a_instruction(void) {
    const char expected[] = 
        "0000000000000000\n" // @0 data
        "0000000000010000\n" // @i new var
        "0000000000010000\n" // var in symbols table
        "0000000000010001\n" // @n another new var
        "0111111111111111\n"; // @32767 data

    // a instruction is passed without @
    write_a_instruction("0", output_file);
    write_a_instruction("i", output_file);
    write_a_instruction("i", output_file);
    write_a_instruction("n", output_file);
    write_a_instruction("32767", output_file);

    rewind(output_file);
    memset(line, '\0', TEST_MAX_LINE_LENGHT);
    read(fd_output_file, line, TEST_MAX_LINE_LENGHT);
    TEST_ASSERT_EQUAL_STRING(expected, line); 
}

void test__write_jump_instruction(void) {
    // c-instruction a-bit comp dest jump
    // 111 0 ?????? 000 ???
    const char expected[] =
        "1110101010000111\n" // 0;JMP
        "1110001100000001\n" // D;JGT
        "1110001100000010\n" // D;JEQ
        "1110001100000011\n" // D;JGE
        "1110001100000100\n" // D;JLT
        "1110001100000101\n" // D;JNE
        "1110001100000110\n" // D;JLE
        "1110001100000111\n"; // D;JMP

    _write_jump_instruction("0;JMP", output_file);
    _write_jump_instruction("D;JGT", output_file);
    _write_jump_instruction("D;JEQ", output_file);
    _write_jump_instruction("D;JGE", output_file);
    _write_jump_instruction("D;JLT", output_file);
    _write_jump_instruction("D;JNE", output_file);
    _write_jump_instruction("D;JLE", output_file);
    _write_jump_instruction("D;JMP", output_file);

    rewind(output_file);
    memset(line, '\0', TEST_MAX_LINE_LENGHT);
    read(fd_output_file, line, TEST_MAX_LINE_LENGHT);
    TEST_ASSERT_EQUAL_STRING(expected, line);
}

void test__write_assignment_instruction(void) {
    // non exhaustive list, append specific test if something breaks

    // c-instruction a-bit comp dest jump
    // 111 ? ?????? ??? 000
    const char expected[] = 
        "1110101010010000\n" // D=0
        "1111110111100000\n" // A=M+1
        "1111110010101000\n" // AM=M-1
        "1110110000010000\n" // D=A
        "1111110001001000\n"; // M=!M

    _write_assignment_instruction("D=0", output_file);
    _write_assignment_instruction("A=M+1", output_file);
    _write_assignment_instruction("AM=M-1", output_file);
    _write_assignment_instruction("D=A", output_file);
    _write_assignment_instruction("M=!M", output_file);

    rewind(output_file);
    memset(line, '\0', TEST_MAX_LINE_LENGHT);
    read(fd_output_file, line, TEST_MAX_LINE_LENGHT);
    TEST_ASSERT_EQUAL_STRING(expected, line);
}

void test_write_c_instruction(void) {
    const char expected[] =
        "1110101010000111\n" // 0;JMP
        "1110110000010000\n"; // D=A

    write_c_instruction("0;JMP", output_file);
    write_c_instruction("D=A", output_file);

    rewind(output_file);
    memset(line, '\0', TEST_MAX_LINE_LENGHT);
    read(fd_output_file, line, TEST_MAX_LINE_LENGHT);
    TEST_ASSERT_EQUAL_STRING(expected, line);
}
