#ifndef DYNAMIC_STRING_ARRAY_H
#define DYNAMIC_STRING_ARRAY_H

#define ARRAY_INIT_SIZE 20
#define ARRAY_UPDATE_SIZE 10

struct DynamicStringArray {
    char **stringArray;
    int count;
    int size;

    void (*put)(struct DynamicStringArray *self, const char *value);
    int (*find)(struct DynamicStringArray *self, const char *value, int exact);
    void (*dump)(struct DynamicStringArray *self);
    void (*del)(struct DynamicStringArray *self);
};

struct DynamicStringArray *DynamicStringArray_new(void);
void __put(struct DynamicStringArray *self, const char *value);
int __find(struct DynamicStringArray *self, const char *value, int exact);
void __dump(struct DynamicStringArray *self);
void __del(struct DynamicStringArray *self);

#endif // DYNAMIC_STRING_ARRAY_H
