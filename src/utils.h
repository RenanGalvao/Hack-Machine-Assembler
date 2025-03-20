#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <regex.h>

// string
void strip(char *s);
int is_empty_string(char *s);
void reverse(char *s);
void decimal_to_binary(int d, char *s, int size, int pad);
int is_numeric_string(char *s);
void remove_spaces(char *s);
void clear_line(char *s);
char **explode(const char *source, const char *delimiter);
void free_exploded(char **arr);
int arrlen(char **arr);
void re_match(const char *regex, const char *string, regmatch_t pmatch[]);
char *get_file_name(char *file_path);

// file
FILE *open_source_file(const char *file_path);
FILE *open_output_file(char *file_path, FILE *source_file);
void write_output(const char *line, FILE *output_file);

#endif // UTILS_H
