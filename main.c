#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "tables.h"

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Usage: hack-assembler <source-file>\n");
        exit(1);
    }

    // source file
    FILE *source_file = fopen(argv[1], "r");
    if(source_file == NULL) {
        perror("Error opening source file");
        exit(1);
    }

    // output file
    char out[128];
    char *c = strchr(argv[1], '.');
    if (c) {
        *c = '\0';
    }
    snprintf(out, 128, "%s.hack", argv[1]);

    FILE *output_file = fopen(out, "w");
    if(output_file == NULL) {
        perror("Error opening output file");
        fclose(source_file);
        exit(1);
    }

    // Initialization
    struct HashMap *symbols_table = get_symbols_table();

    // get labels
    first_scan(source_file, symbols_table);

    // work out source code
    if(second_scan(source_file, symbols_table, output_file) != 0) {
        // error, erase output file
        remove(out);
    }

    // shutdown
    symbols_table->del(symbols_table);
    fclose(source_file);
    fclose(output_file);

    return 0;
}
