#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic-string-array.h"
#include "regex.h"
#include "utils.h"

struct DynamicStringArray *DynamicStringArray_new(void) {
    struct DynamicStringArray *p = NULL;
    p = malloc(sizeof(struct DynamicStringArray));
    p->stringArray = malloc(ARRAY_INIT_SIZE * sizeof(p->stringArray));
    p->count = 0;
    p->size = ARRAY_INIT_SIZE;

    p->put = &__put;
    p->find = &__find;
    p->dump = &__dump;
    p->del = &__del;

    return p;
}

void __increaseSize(struct DynamicStringArray *self) {
    self->size += ARRAY_UPDATE_SIZE;
    self->stringArray = realloc(self->stringArray, self->size * sizeof(self->stringArray));
}

void __put(struct DynamicStringArray *self, const char *value) {
    if(self->count < self->size) {
        self->stringArray[self->count] = malloc(sizeof(char) * (strlen(value) + 1));
        strcpy(self->stringArray[self->count], value);
        self->count++;
    } else {
        __increaseSize(self);
        __put(self, value);
    }
}

int __find(struct DynamicStringArray *self, const char *value, int exact) {
    for(int i = 0; i < self->count; i++) {
        #ifdef DEBUG
        printf("[dynamic-string-array.c]: __find self->stringArray[i] '%s'\n", self->stringArray[i]);
        #endif // DEBUG

        if(exact) {
            char tmp[128];
            strcpy(tmp, self->stringArray[i]);
            clearLine(tmp);

            if(strcmp(value, tmp) == 0) {
                return i;
            }
        } else {
            regmatch_t pmatch[1];
            re_match(value, self->stringArray[i], pmatch);

            int off = pmatch[0].rm_so;

            if(off != -1) {
                return i;
            }
        }
    }

    return -1;
}

void __dump(struct DynamicStringArray *self) {
    for(int i = 0; i < self->count; i++) {
        printf("%s\n", self->stringArray[i]);
    }
}

void __del(struct DynamicStringArray *self) {
    for(int i = 0; i < self->count; i++) {
        free(self->stringArray[i]);
    }
    free(self->stringArray);
    free((void *)self);
}
