#include "parser.h"
#include "tables.h"
#include "defs.h"
#include "utils.h"
#include "unity.h"
#include "mock_code.h"
#include <stdint.h>
#include <stdio.h>

// force to incluse hash-map.c
TEST_SOURCE_FILE("hash-map.c");

struct HashMap *symbols_table = NULL;
struct HashMap *comp_table = NULL;
struct HashMap *dest_table = NULL;
struct HashMap *jump_table = NULL;

// second scan
FILE *output_file = NULL;

void setUp(void) {
    symbols_table = get_symbols_table();
    comp_table = get_comp_table();
    dest_table = get_dest_table();
    jump_table = get_jump_table();

    // second scan
    const char output_file_path[] = "test/files/second_scan_output.hack";
    output_file = fopen(output_file_path, "w");
}

void tearDown(void) {
    symbols_table->del(symbols_table);
    comp_table->del(comp_table);
    dest_table->del(dest_table);
    jump_table->del(jump_table);

    //second scan
    fclose(output_file);
}

void test_first_scan(void) {
    // should not be increased
    const char no_label_file_path[] = "test/files/first_scan_no_label.asm";
    FILE *no_label_file = fopen(no_label_file_path, "r");
    first_scan(no_label_file);
    const int default_labels_count = 23;
    TEST_ASSERT_EQUAL(default_labels_count, symbols_table->size(symbols_table));
    fclose(no_label_file);

    // should increase by 3
    const char labels_file_path[] = "test/files/first_scan_3_labels.asm";
    FILE *labels_file = fopen(labels_file_path, "r");
    first_scan(labels_file);
    TEST_ASSERT_EQUAL(default_labels_count + 3, symbols_table->size(symbols_table));
    fclose(labels_file);
}

void test_second_scan(void) {
    // A instruction should fail
    // see second_scan_a_fail.asm file
    const char a_fail_file_path[] = "test/files/second_scan_a_fail.asm";
    FILE *a_fail_file = fopen(a_fail_file_path, "r");

    // empty @
    TEST_ASSERT_EQUAL(-1, second_scan(a_fail_file, output_file));
    // @32768 is bigger than 2^15-1 = MAX_ADDRESS
    TEST_ASSERT_EQUAL(-1, second_scan(a_fail_file, output_file));
    // @-100 invalid address
    TEST_ASSERT_EQUAL(-1, second_scan(a_fail_file, output_file));
    fclose(a_fail_file);


    // A instruction should succeed
    // it will pass instruction without @
    // see second_scan_a_success.asm file
    const char a_success_file_path[] = "test/files/second_scan_a_success.asm";
    FILE *a_success_file = fopen(a_success_file_path, "r");

    write_a_instruction_Expect("0", output_file);
    write_a_instruction_Expect("32767", output_file);
    write_a_instruction_Expect("Foo", output_file);
    write_a_instruction_Expect("Foo.i", output_file);
    write_a_instruction_Expect("$_1", output_file);
    TEST_ASSERT_EQUAL(0, second_scan(a_success_file, output_file));
    fclose(a_success_file);


    // C instruction-JUMP should fail
    // see second_scan_c_jump_fail.asm file
    const char c_jump_fail_file_path[] = "test/files/second_scan_c_jump_fail.asm";
    FILE *c_jump_fail_file = fopen(c_jump_fail_file_path, "r");

    // ;
    TEST_ASSERT_EQUAL(-1, second_scan(c_jump_fail_file, output_file));
    // D;
    TEST_ASSERT_EQUAL(-1, second_scan(c_jump_fail_file, output_file));
    // ;JMP
    TEST_ASSERT_EQUAL(-1, second_scan(c_jump_fail_file, output_file));
    // B;JMP
    TEST_ASSERT_EQUAL(-1, second_scan(c_jump_fail_file, output_file));
    // D;JEMP
    TEST_ASSERT_EQUAL(-1, second_scan(c_jump_fail_file, output_file));
    fclose(c_jump_fail_file);


    // C instruction-JUMP should succeed
    // see second_scan_c_jump_success.asm file
    const char c_jump_success_file_path[] = "test/files/second_scan_c_jump_success.asm";
    FILE *c_jump_success_file = fopen(c_jump_success_file_path, "r");

    write_c_instruction_Expect("0;JMP", output_file);
    write_c_instruction_Expect("D;JGT", output_file);
    write_c_instruction_Expect("D;JEQ", output_file);
    write_c_instruction_Expect("D;JGE", output_file);
    write_c_instruction_Expect("D;JLT", output_file);
    write_c_instruction_Expect("D;JNE", output_file);
    write_c_instruction_Expect("D;JLE", output_file);
    write_c_instruction_Expect("D;JMP", output_file);
    TEST_ASSERT_EQUAL(0, second_scan(c_jump_success_file, output_file));
    fclose(c_jump_success_file);


    // C instruction-ASSIGNMENT should fail
    // see second_scan_c_assignment_fail.asm file
    const char c_assignment_fail_file_path[] = "test/files/second_scan_c_assignment_fail.asm";
    FILE *c_assignment_fail_file = fopen(c_assignment_fail_file_path, "r");

    // =
    TEST_ASSERT_EQUAL(-1, second_scan(c_assignment_fail_file, output_file));
    // D=
    TEST_ASSERT_EQUAL(-1, second_scan(c_assignment_fail_file, output_file));
    // =M
    TEST_ASSERT_EQUAL(-1, second_scan(c_assignment_fail_file, output_file));
    // C=D
    TEST_ASSERT_EQUAL(-1, second_scan(c_assignment_fail_file, output_file));
    // A=N
    TEST_ASSERT_EQUAL(-1, second_scan(c_assignment_fail_file, output_file));
    fclose(c_assignment_fail_file);


    // C instruction-ASSIGNMENT should succeed
    // see second_scan_c_assignment_success.asm file
    const char c_assignment_success_file_path[] = "test/files/second_scan_c_assignment_success.asm";
    FILE *c_assignment_success_file = fopen(c_assignment_success_file_path, "r");

    write_c_instruction_Expect("D=A", output_file);
    write_c_instruction_Expect("AMD=M+1", output_file);
    write_c_instruction_Expect("MD=M-1", output_file);
    write_c_instruction_Expect("AD=!D", output_file);
    write_c_instruction_Expect("M=A-1", output_file);
    TEST_ASSERT_EQUAL(0, second_scan(c_assignment_success_file, output_file));
    fclose(c_assignment_success_file);


    // Label should succeed - be ignored
    // see second_scan_label_success.asm
    const char label_success_file_path[] = "test/files/second_scan_label_success.asm";
    FILE *label_success_file = fopen(label_success_file_path, "r");

    TEST_ASSERT_EQUAL(0, second_scan(label_success_file, output_file));
}
