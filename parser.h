#ifndef PARSER_H
#define PARSER_H

#include "dynamic-string-array.h"
#include "hash-map.h"

void firstScan(struct DynamicStringArray *cleanSource, struct HashMap *symbolsTable);
int secondScan(struct DynamicStringArray *source, struct DynamicStringArray *cleanSource, struct HashMap *symbolsTable, struct DynamicStringArray *output);
void badInstruction(struct DynamicStringArray *source, const char *s);
void cleanUp(struct HashMap *compTable, struct HashMap *destTable, struct HashMap *jumpTable);

#endif // PARSER_H
