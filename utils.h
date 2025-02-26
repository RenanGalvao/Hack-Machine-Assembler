#ifndef UTILS_H
#define UTILS_H

#include "dynamic-string-array.h"

// string
void rstrip(char *s);
void lstrip(char *s);
void strip(char *s);
int isEmptyString(char *s);
void reverse(char *s);
void decimalToBinaryString(int d, char *s, int size, int pad);
int isNumericString(char *s);
void removeSpaces(char *s);
void clearLine(char *s);

// file
void getSourceCode(char *filePath, struct DynamicStringArray *source, struct DynamicStringArray *cleanSource);
void saveHackCode(char *sourceFile, struct DynamicStringArray *output);

#endif // UTILS_H
