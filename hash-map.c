#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "hash-map.h"

struct HashMap *HashMap_new(void) {
    struct HashMap *p = malloc(sizeof(struct HashMap));
    int i;

    p->__buckets = HASH_MAP_SIZE;
    for(i = 0; i < p->__buckets; i++) {
        p->__heads[i] = NULL;
        p->__tails[i] = NULL;
    }

    p->__count = 0;

    p->put = &__HashMap_put;
    p->get = &__HashMap_get;
    p->size = &__HashMap_size;
    p->dump = &__HashMap_dump;
    p->del = &__HashMap_del;

    p->iter = &__HashMap_iter;

    return p;
}

struct HashMapEntry * __HashMap_find(struct HashMap *self, const char *key)
{
    if(self == NULL || key == NULL) return NULL;

    int bucket;
    struct HashMapEntry *cur;

    bucket = getBucket(key, HASH_MAP_SIZE); 

    for(cur = self->__heads[bucket]; cur != NULL; cur = cur->__next) {
        if(strcmp(key, cur->key) == 0)
            return cur;
    }

    return NULL;
}

void __HashMap_put(struct HashMap *self, const char *key, const char *value) {
    int bucket;
    struct HashMapEntry *old, *new;

    if (key == NULL) return;

    old = __HashMap_find(self, key);
    if( old != NULL) {
        old->value = realloc(old->value, sizeof(char) * (strlen(value) + 1));
        strcpy(old->value, value);
        return;
    }

    new = malloc(sizeof(struct HashMapEntry));
    new->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(new->key, key);
    new->value = malloc(sizeof(char) * (strlen(value) + 1));
    strcpy(new->value, value);
    new->__next = NULL;

    bucket = getBucket(key, HASH_MAP_SIZE);

    if(self->__heads[bucket] == NULL)
        self->__heads[bucket] = new;

    if(self->__tails[bucket] != NULL) {
        self->__tails[bucket]->__next = new;
        new->__prev = self->__tails[bucket];
    }

    self->__tails[bucket] = new;
    self->__count++;
}

char *__HashMap_get(struct HashMap *self, const char *key) {
    if(self == NULL || key == NULL) return NULL;

    struct HashMapEntry *mapEntry;

    mapEntry = __HashMap_find(self, key);
    if(mapEntry != NULL)
        return mapEntry->value;
    else
        return NULL;
}

int __HashMap_size(struct HashMap *self) {
    if (self == NULL) return -1;

    return self->__count;
}

void __HashMap_del(struct HashMap *self) {
    int i;
    struct HashMapEntry *cur, *next;

    for(i = 0; i < self->__buckets; i++) {
        cur = self->__heads[i];
        while(cur) {
            free(cur->key);
            free(cur->value);
            next = cur->__next;
            free(cur);
            cur = next;
        }
    }
    free((void *)self);
}

void __HashMap_dump(struct HashMap *self) {
    int i;
    struct HashMapEntry *cur;

    printf("Object HashMap count=%d buckets=%d\n", self->__count, self->__buckets);
    for(i = 0; i < self->__buckets; i++)
        for(cur = self->__heads[i]; cur != NULL; cur = cur->__next)
            printf("  %s=%s [%d]\n", cur->key, cur->value, i);
}


struct HashMapIter *__HashMap_iter(struct HashMap *map) {
    struct HashMapIter *p = malloc(sizeof(struct HashMapIter));
    p->__map = map;
    p->__bucket = 0;
    p->__current = map->__heads[p->__bucket];
    p->next = &__HashMapIter_next;
    p->del = &__HashMapIter_del;

    return p;
}

struct HashMapEntry *__HashMapIter_next(struct HashMapIter *self) {
    struct HashMapEntry *retval;

    /* scan for a bucket that is not null */
    while(self->__current == NULL) {
        self->__bucket++;
        if(self->__bucket >= self->__map->__buckets) return NULL;
        self->__current = self->__map->__heads[self->__bucket];
    }

    retval = self->__current;
    if(self->__current != NULL)
        self->__current = self->__current->__next;

    return retval;
}

void __HashMapIter_del(struct HashMapIter *self) {
    free((void*)self);
}

int getBucket(const char *str, int buckets) {
    int hash = 42;
    if (str == NULL) return 0;
    for( ; *str; str++)
        hash = (hash << 3) ^ *str;

    return abs(hash % buckets);
}
