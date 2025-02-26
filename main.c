#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "tables.h"
#include "dynamic-string-array.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Usage: hack-assembler <source-file>\n");
        exit(1);
    }

    // Initialization
    struct HashMap *symbolsTable = getSymbolsTable();
    struct DynamicStringArray *source = DynamicStringArray_new();
    struct DynamicStringArray *cleanSource = DynamicStringArray_new();
    struct DynamicStringArray *output = DynamicStringArray_new();
    getSourceCode(argv[1], source, cleanSource);

    // get symbols
    firstScan(cleanSource, symbolsTable);
    // work out source code
    if(secondScan(source, cleanSource, symbolsTable, output) == 0) {
        //success
        saveHackCode(argv[1], output);
    }

    // shutdown
    symbolsTable->del(symbolsTable);
    source->del(source);
    cleanSource->del(cleanSource);
    output->del(output);

    return 0;
}
