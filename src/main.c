#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "tables.h"
#include "utils.h"

struct HashMap *symbols_table = NULL;
struct HashMap *comp_table = NULL;
struct HashMap *dest_table = NULL; 
struct HashMap *jump_table = NULL;

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Usage: hack-assembler <source-file>\n");
        exit(1);
    }

    FILE *source_file = open_source_file(argv[1]);
    FILE *output_file = open_output_file(argv[1], source_file);
    char *out = get_file_name(argv[1]); 

    // Initialization
    symbols_table = get_symbols_table();
    comp_table = get_comp_table();
    dest_table = get_dest_table();
    jump_table = get_jump_table();

    // get labels
    first_scan(source_file);

    // work out source code
    if(second_scan(source_file, output_file) != 0) {
        // error, erase output file
        remove(out);
    }

    // shutdown
    symbols_table->del(symbols_table);
    comp_table->del(comp_table);
    dest_table->del(dest_table);
    jump_table->del(jump_table);
    fclose(source_file);
    fclose(output_file);
    free(out);

    return 0;
}
