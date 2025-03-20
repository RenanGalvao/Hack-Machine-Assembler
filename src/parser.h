#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

void first_scan(FILE *source_file);
int second_scan(FILE *source_file, FILE *output_file);
void bad_instruction(const char *line, int line_number);

#endif // PARSER_H
