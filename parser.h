#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "hash-map.h"

void first_scan(FILE *source_file, struct HashMap *symbols_table);
int second_scan(FILE *source_file, struct HashMap *symbols_table, FILE *output_file);
void bad_instruction(const char *line, int line_number);
void clean_up(struct HashMap *comp_table, struct HashMap *dest_table, struct HashMap *jump_table);

#endif // PARSER_H
