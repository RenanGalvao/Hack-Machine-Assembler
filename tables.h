#ifndef TABLES_H
#define TABLES_H

#include "hash-map.h"

struct HashMap *getSymbolsTable(void);
struct HashMap *getDestTable(void);
struct HashMap *getCompTable(void);
struct HashMap *getJumpTable(void);

#endif // TABLES_H
