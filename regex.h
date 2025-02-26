#ifndef REGEX_H
#define REGEX_H

#include <regex.h>

void re_match(const char *regex, const char *string, regmatch_t pmatch[]);

#endif // REGEX_H
