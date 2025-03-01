#include <stdio.h>
#include "tables.h"
#include "utils.h"

struct HashMap *get_symbols_table(void) {
  struct HashMap *symbols_table = HashMap_new();
  for (int i = 0; i < 16; i++) {
    char symbol[4];
    char value[17];
    snprintf(symbol, 4, "R%d", i);
    decimal_to_binary(i, value, 17, 1);
    symbols_table->put(symbols_table, symbol, value);
  }
  symbols_table->put(symbols_table, "SCREEN", "0100000000000000");
  symbols_table->put(symbols_table, "KBD", "0110000000000000");
  symbols_table->put(symbols_table, "SP", "0000000000000000");
  symbols_table->put(symbols_table, "LCL", "0000000000000001");
  symbols_table->put(symbols_table, "ARG", "0000000000000010");
  symbols_table->put(symbols_table, "THIS", "0000000000000011");
  symbols_table->put(symbols_table, "THAT", "0000000000000100");

  return symbols_table;
}

struct HashMap *get_comp_table(void) {
  struct HashMap *comp_table = HashMap_new();

  comp_table->put(comp_table, "0", "101010");
  comp_table->put(comp_table, "1", "111111");
  comp_table->put(comp_table, "-1", "111010");
  comp_table->put(comp_table, "D", "001100");
  comp_table->put(comp_table, "A", "110000");
  comp_table->put(comp_table, "M", "110000");
  comp_table->put(comp_table, "!D", "001101");
  comp_table->put(comp_table, "!A", "110001");
  comp_table->put(comp_table, "!M", "110001");
  comp_table->put(comp_table, "-D", "001111");
  comp_table->put(comp_table, "-A", "110011");
  comp_table->put(comp_table, "-M", "110011");
  comp_table->put(comp_table, "D+1", "011111");
  comp_table->put(comp_table, "A+1", "110111");
  comp_table->put(comp_table, "M+1", "110111");
  comp_table->put(comp_table, "D-1", "001110");
  comp_table->put(comp_table, "A-1", "110010");
  comp_table->put(comp_table, "M-1", "110010");
  comp_table->put(comp_table, "D+A", "000010");
  comp_table->put(comp_table, "D+M", "000010");
  comp_table->put(comp_table, "D-A", "010011");
  comp_table->put(comp_table, "D-M", "010011");
  comp_table->put(comp_table, "A-D", "000111");
  comp_table->put(comp_table, "M-D", "000111");
  comp_table->put(comp_table, "D&A", "000000");
  comp_table->put(comp_table, "D&M", "000000");
  comp_table->put(comp_table, "D|A", "010101");
  comp_table->put(comp_table, "D|M", "010101");

  return comp_table;
}


struct HashMap *get_dest_table(void) {
    struct HashMap *dest_table = HashMap_new();

    // maybe a better find?
    // for dest, set bit for every dest 
    dest_table->put(dest_table, "M", "001");
    dest_table->put(dest_table, "D", "010");
    dest_table->put(dest_table, "DM", "011");
    dest_table->put(dest_table, "MD", "011");
    dest_table->put(dest_table, "A", "100");
    dest_table->put(dest_table, "AM", "101");
    dest_table->put(dest_table, "MA", "101");
    dest_table->put(dest_table, "AD", "110");
    dest_table->put(dest_table, "DA", "110");
    dest_table->put(dest_table, "ADM", "111");
    dest_table->put(dest_table, "ADM", "111");
    dest_table->put(dest_table, "ADM", "111");
    dest_table->put(dest_table, "ADM", "111");
    dest_table->put(dest_table, "ADM", "111");
    dest_table->put(dest_table, "ADM", "111");

    return dest_table;
}

struct HashMap *get_jump_table(void) {
    struct HashMap *jump_table = HashMap_new();

    jump_table->put(jump_table, "JGT", "001");
    jump_table->put(jump_table, "JEQ", "010");
    jump_table->put(jump_table, "JGE", "011");
    jump_table->put(jump_table, "JLT", "100");
    jump_table->put(jump_table, "JNE", "101");
    jump_table->put(jump_table, "JLE", "110");
    jump_table->put(jump_table, "JMP", "111");

    return jump_table;
}
