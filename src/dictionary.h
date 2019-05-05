#ifndef _DICTIONARY_H
#define _DICTIONARY_H

#include <stdint.h>
#include <stdlib.h>

struct dict_element
{
    uint32_t hkey;
    uint32_t key;
    long value;
    struct dict_element *next;
};

struct dict
{
    size_t capacity;
    size_t size;
    struct dict_element* data;
};

typedef struct dict_element* dict_result;
typedef struct dict* dictionary;

uint32_t hash(void *key, size_t len);

struct dict* dict_new(size_t capacity);

void dict_clear(struct dict *d);
void dict_free(struct dict *d);

struct dict_element *dict_get(struct dict *d, uint32_t key);
int dict_insert(struct dict *d, uint32_t key, long value);
void dict_remove(struct dict *d, uint32_t key);

//void dict_print(struct dict *d);
//
#endif
