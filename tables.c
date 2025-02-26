#include <stdio.h>
#include "tables.h"
#include "utils.h"

struct HashMap *getSymbolsTable(void) {
  struct HashMap *symbolsTable = HashMap_new();
  for (int i = 0; i < 16; i++) {
    char symbol[4];
    char value[17];
    snprintf(symbol, 4, "R%d", i);
    decimalToBinaryString(i, value, 17, 1);
    symbolsTable->put(symbolsTable, symbol, value);
  }
  symbolsTable->put(symbolsTable, "SCREEN", "0100000000000000");
  symbolsTable->put(symbolsTable, "KBD", "0110000000000000");
  symbolsTable->put(symbolsTable, "SP", "0000000000000000");
  symbolsTable->put(symbolsTable, "LCL", "0000000000000001");
  symbolsTable->put(symbolsTable, "ARG", "0000000000000010");
  symbolsTable->put(symbolsTable, "THIS", "0000000000000011");
  symbolsTable->put(symbolsTable, "THAT", "0000000000000100");

  return symbolsTable;
}

struct HashMap *getCompTable(void) {
  struct HashMap *compTable = HashMap_new();

  compTable->put(compTable, "0", "101010");
  compTable->put(compTable, "1", "111111");
  compTable->put(compTable, "-1", "111010");
  compTable->put(compTable, "D", "001100");
  compTable->put(compTable, "A", "110000");
  compTable->put(compTable, "M", "110000");
  compTable->put(compTable, "!D", "001101");
  compTable->put(compTable, "!A", "110001");
  compTable->put(compTable, "!M", "110001");
  compTable->put(compTable, "-D", "001111");
  compTable->put(compTable, "-A", "110011");
  compTable->put(compTable, "-M", "110011");
  compTable->put(compTable, "D+1", "011111");
  compTable->put(compTable, "A+1", "110111");
  compTable->put(compTable, "M+1", "110111");
  compTable->put(compTable, "D-1", "001110");
  compTable->put(compTable, "A-1", "110010");
  compTable->put(compTable, "M-1", "110010");
  compTable->put(compTable, "D+A", "000010");
  compTable->put(compTable, "D+M", "000010");
  compTable->put(compTable, "D-A", "010011");
  compTable->put(compTable, "D-M", "010011");
  compTable->put(compTable, "A-D", "000111");
  compTable->put(compTable, "M-D", "000111");
  compTable->put(compTable, "D&A", "000000");
  compTable->put(compTable, "D&M", "000000");
  compTable->put(compTable, "D|A", "010101");
  compTable->put(compTable, "D|M", "010101");

  return compTable;
}


struct HashMap *getDestTable(void) {
    struct HashMap *destTable = HashMap_new();

    // maybe a better find?
    // for dest, set bit for every dest 
    destTable->put(destTable, "M", "001");
    destTable->put(destTable, "D", "010");
    destTable->put(destTable, "DM", "011");
    destTable->put(destTable, "MD", "011");
    destTable->put(destTable, "A", "100");
    destTable->put(destTable, "AM", "101");
    destTable->put(destTable, "MA", "101");
    destTable->put(destTable, "AD", "110");
    destTable->put(destTable, "DA", "110");
    destTable->put(destTable, "ADM", "111");
    destTable->put(destTable, "ADM", "111");
    destTable->put(destTable, "ADM", "111");
    destTable->put(destTable, "ADM", "111");
    destTable->put(destTable, "ADM", "111");
    destTable->put(destTable, "ADM", "111");

    return destTable;
}

struct HashMap *getJumpTable(void) {
    struct HashMap *jumpTable = HashMap_new();

    jumpTable->put(jumpTable, "JGT", "001");
    jumpTable->put(jumpTable, "JEQ", "010");
    jumpTable->put(jumpTable, "JGE", "011");
    jumpTable->put(jumpTable, "JLT", "100");
    jumpTable->put(jumpTable, "JNE", "101");
    jumpTable->put(jumpTable, "JLE", "110");
    jumpTable->put(jumpTable, "JMP", "111");

    return jumpTable;
}
