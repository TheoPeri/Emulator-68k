#include "dictionary.h"

// Jenkins's one_at_a_time hash algorithm
uint32_t hash(void *value, size_t len)
{
    unsigned char* key = (unsigned char*)value;
    uint32_t hash = 0;
    size_t i;

    for (i = 0; i < len; i++)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

struct dict* dict_new(size_t capacity)
{
    //Allocating our dictionary
    struct dict* dict = malloc(sizeof(struct dict));
    
    //Initialize the structure's properties
    dict->capacity = capacity;
    dict->size = 0;
    dict->data = malloc(capacity * sizeof(struct dict_element));
    
    //Initialize our array based on capacity
    for(size_t i = 0; i < capacity; i++)
    {
        dict->data[i].hkey = 0;
        dict->data[i].key = 0;
        dict->data[i].value = 0;
        dict->data[i].next = NULL;
    }
    return dict;
}

void dict_clear(struct dict *d)
{
    //Iterate through all the elements to delete them
    for(size_t i = 0; i < d->size; i++)
    {
        struct dict_element* list = &d->data[i];
        //Free all the elements inside the local list
        for(;list;)
        {
            struct dict_element* net = list->next;
            free(list);
            list = net;
        }
    }
    d->size = 0;
}

void dict_free(struct dict *d)
{
    //Clear the data
    dict_clear(d);
    //Free the dictionary's table
    free(d->data);
    //Free the dictionary
    free(d);
}

struct dict_element *dict_get(struct dict *d, uint32_t key)
{
    //Hash our key
    uint32_t h = hash(&key, 4);
    //Calculate the position on our array
    uint32_t i = h % d->capacity;
    
    //Search for the element inside the array
    struct dict_element* list = d->data[i].next;
    for(;list; list = list->next)
        if(list->hkey == h) return list;
    
    return NULL;
}

void recalculate_indexes(struct dict *d, struct dict_element* newData, size_t newcap)
{
    for(size_t j = 0; j < d->capacity; j++)
    {
        struct dict_element* list = d->data[j].next;
        for(;list;)
        {
            size_t newIndex = list->hkey % newcap;
            void* localnext = list->next;
            
            list->next = newData[newIndex].next;
            newData[newIndex].next = list;
            
            list = localnext;
        }
    }
}

int dict_insert(struct dict *d, uint32_t key, long value)
{
    //Hash our key
    uint32_t h = hash(&key, 4);
    //Calculate the position on our array
    uint32_t i = h % d->capacity;
    
    //Make sure the key doesn't already exist
    struct dict_element* list = &d->data[i];
    //Since we are at it, get the last place in the list
    struct dict_element* last = list;
    
    for(;list; list = list->next)
    {
        if(list->hkey == h) return 0;
        last = list;
    }
    
    //Calculate how full we are
    int ratio = 100 * d->size / d->capacity;
    
    struct dict_element* element = malloc(sizeof(struct dict_element));
    
    element->hkey = h;
    element->value = value;
    element->key = key;
    element->next = NULL;
    
    last->next = element;
    
    d->size++;
    
    //If our array is 75% full reallocate more space
    if(ratio > 75)
    {
        size_t newcap = d->capacity * 2;
        struct dict_element* newData = calloc(newcap, sizeof(struct dict_element));
        
        //go through the existing elements and arrange them accordingly
        recalculate_indexes(d, newData, newcap);
        
        free(d->data);
        
        d->capacity = newcap;
        d->data = newData;
    }
    
    return 1;
}

void dict_remove(struct dict *d, uint32_t key)
{
    uint32_t h = hash(&key, 4);
    uint32_t i = h % d->capacity;
    
    //Current list corresponding to the hash key
    struct dict_element* list = d->data[i].next;
    
    //Keep track of the last element so we can join
    //the list after removing an element
    struct dict_element* last = &d->data[i];
    
    //Look for the element with the same key
    for(;list; list = list->next)
    {
        //Does it have the same key?
        if(list->hkey == h)
        {
            //Reconnect the list
            if(last) last->next = list->next;
            else d->data[i] = *list->next;
            
            //Free the removed element
            free(list);
            break;
        }
        last = list;
    }
    d->size--;
}

/*void dict_print(struct dict *d)
{
    printf("==================\n");
    for(size_t i = 0; i < d->capacity; i++)
    {
        printf("[%d]", i);
        struct dict_element* list = d->data[i].next;
        for(;list; list = list->next)
            printf(" -> (%d, %d, %d)", 
                    list->hkey, 
                    list->key, 
                    (uint32_t)list->value);
        printf("\n");
    }
    printf("==================\n");
}*/
