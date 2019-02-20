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

//Hashing function
uint32_t hash(void *key, size_t len);

//Create a new dictionary
struct dict* dict_new(size_t capacity);

//Erase all the current data, dictionary still usable
void dict_clear(struct dict *d);

//Free the dictionary's memory, it becomes unusable
void dict_free(struct dict *d);

//Get the element corresponding to the key
//returns NULL if the key doesn't exist
struct dict_element *dict_get(struct dict *d, uint32_t key);

//Insert an element into the dictionary
//returns 0 if the key already exists
int dict_insert(struct dict *d, uint32_t key, long value);

//Remove the element with the corresponding key
void dict_remove(struct dict *d, uint32_t key);

//void dict_print(struct dict *d);

#endif
