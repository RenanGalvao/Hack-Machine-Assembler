#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

// string
void strip(char *s);
int is_empty_string(char *s);
void reverse(char *s);
void decimal_to_binary(int d, char *s, int size, int pad);
int is_numeric_string(char *s);
void remove_spaces(char *s);
void clear_line(char *s);

// file
void write_output(const char *line, FILE *output_file);

#endif // UTILS_H
