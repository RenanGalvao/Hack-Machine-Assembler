#include <stdio.h>
#include "tables.h"

struct HashMap *get_symbols_table(void) {
  struct HashMap *symbols_table = HashMap_new();
  for (int i = 0; i < 16; i++) {
    char symbol[4];
    snprintf(symbol, 4, "R%d", i);
    symbols_table->put(symbols_table, symbol, i);
  }
  symbols_table->put(symbols_table, "SCREEN", 16384); // "0100000000000000"
  symbols_table->put(symbols_table, "KBD", 24576); // "0110000000000000"
  symbols_table->put(symbols_table, "SP", 0); // "0000000000000000"
  symbols_table->put(symbols_table, "LCL", 1); // "0000000000000001"
  symbols_table->put(symbols_table, "ARG", 2); // "0000000000000010"
  symbols_table->put(symbols_table, "THIS", 3); // "0000000000000011"
  symbols_table->put(symbols_table, "THAT", 4); // "0000000000000100"

  return symbols_table;
}

struct HashMap *get_comp_table(void) {
  struct HashMap *comp_table = HashMap_new();
  // word size = 16
  // all comp values are right offset by dest + jump
  // this means left shift by 6
  comp_table->put(comp_table, "0", 42 << 6); // "101010"
  comp_table->put(comp_table, "1", 63 << 6 ); // "111111"
  comp_table->put(comp_table, "-1", 58 << 6); // "111010"
  comp_table->put(comp_table, "D", 12 << 6); // "001100"
  comp_table->put(comp_table, "A", 48 << 6); // "110000"
  comp_table->put(comp_table, "M", 48 << 6); // "110000"
  comp_table->put(comp_table, "!D", 13 << 6); // "001101"
  comp_table->put(comp_table, "!A", 49 << 6); // "110001"
  comp_table->put(comp_table, "!M", 49 << 6); // "110001"
  comp_table->put(comp_table, "-D", 15 << 6); // "001111"
  comp_table->put(comp_table, "-A", 51 << 6); // "110011"
  comp_table->put(comp_table, "-M", 51 << 6); // "110011"
  comp_table->put(comp_table, "D+1", 31 << 6); // "011111"
  comp_table->put(comp_table, "A+1", 55 << 6); // "110111"
  comp_table->put(comp_table, "M+1", 55 << 6); // "110111"
  comp_table->put(comp_table, "D-1", 14 << 6); // "001110"
  comp_table->put(comp_table, "A-1", 50 << 6); // "110010"
  comp_table->put(comp_table, "M-1", 50 << 6); // "110010"
  comp_table->put(comp_table, "D+A", 2 << 6); // "000010"
  comp_table->put(comp_table, "D+M", 2 << 6); // "000010"
  comp_table->put(comp_table, "D-A", 19 << 6); // "010011"
  comp_table->put(comp_table, "D-M", 19 << 6); // "010011"
  comp_table->put(comp_table, "A-D", 7 << 6); // "000111"
  comp_table->put(comp_table, "M-D", 7 << 6); // "000111"
  comp_table->put(comp_table, "D&A", 0); // "000000"
  comp_table->put(comp_table, "D&M", 0); // "000000"
  comp_table->put(comp_table, "D|A", 21 << 6); // "010101"
  comp_table->put(comp_table, "D|M", 21 << 6); // "010101"

  return comp_table;
}


struct HashMap *get_dest_table(void) {
    struct HashMap *dest_table = HashMap_new();

    // maybe a better find?
    // for dest, set bit for every dest 

    // word size = 16
    // all comp values are right offset by jump
    // this means left shift by 3
    dest_table->put(dest_table, "M", 1 << 3); // "001"
    dest_table->put(dest_table, "D", 2 << 3); // "010"
    dest_table->put(dest_table, "DM", 3 << 3); // "011"
    dest_table->put(dest_table, "MD", 3 << 3); // "011"
    dest_table->put(dest_table, "A", 4 << 3); // "100"
    dest_table->put(dest_table, "AM", 5 << 3); // "101"
    dest_table->put(dest_table, "MA", 5 << 3); // "101"
    dest_table->put(dest_table, "AD", 6 << 3); // "110"
    dest_table->put(dest_table, "DA", 6 << 3); // "110"
    dest_table->put(dest_table, "ADM", 7 << 3); // "111"
    dest_table->put(dest_table, "AMD", 7 << 3); // "111"
    dest_table->put(dest_table, "DAM", 7 << 3); // "111"
    dest_table->put(dest_table, "DMA", 7 << 3); // "111"
    dest_table->put(dest_table, "MAD", 7 << 3); // "111"
    dest_table->put(dest_table, "MDA", 7 << 3); // "111"

    return dest_table;
}

struct HashMap *get_jump_table(void) {
    struct HashMap *jump_table = HashMap_new();

    // maybe use an array and find same string, get id as value
    jump_table->put(jump_table, "JGT", 1); // "001"
    jump_table->put(jump_table, "JEQ", 2); // "010"
    jump_table->put(jump_table, "JGE", 3); // "011"
    jump_table->put(jump_table, "JLT", 4); // "100" 
    jump_table->put(jump_table, "JNE", 5); // "101"
    jump_table->put(jump_table, "JLE", 6); // "110"
    jump_table->put(jump_table, "JMP", 7); // "111"

    return jump_table;
}
