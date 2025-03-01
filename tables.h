#ifndef TABLES_H
#define TABLES_H

#include "hash-map.h"

struct HashMap *get_symbols_table(void);
struct HashMap *get_dest_table(void);
struct HashMap *get_comp_table(void);
struct HashMap *get_jump_table(void);

#endif // TABLES_H
