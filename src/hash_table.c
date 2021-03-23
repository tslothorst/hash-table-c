#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "hash_table.h"
#include "prime.h"

const int HT_INITIAL_BASE_SIZE = 29;
ht_item HT_DELETED_ITEM;
error errormessage;

static ht_item* ht_new_item(const char* k, const char* v);
static void ht_del_item(ht_item* i);
void ht_del_hash_table(ht_hash_table* ht);
static int ht_hash(const char* s, const int a, const int m);
static int ht_get_hash(const char* s, const int num_buckets, const int attempt);
char* ht_search(ht_hash_table* ht, const char* key);
static void ht_delete(ht_hash_table* ht, const char* key);
static ht_hash_table* ht_new_sized(const int base_size);
ht_hash_table* ht_new();
static void ht_resize(ht_hash_table* ht, const int base_size);
static void ht_resize_up(ht_hash_table* ht);
static  void ht_resize_down(ht_hash_table* ht);
void ht_insert(ht_hash_table* ht, const char* key, const char* value);

int ht_random_prime(){
    // Call srand and seed it with the current system time so we get a random number every time.
    srand(time(0));
    // We generate a random number between 128 (min value for ASCII chars) and 8192 as upper limit.
    // Then we get the closest prime number after that random number.
    return next_prime((rand() % (8192 - 128 + 1)) + 128);
}

static ht_item* ht_new_item(const char* k, const char* v){
    ht_item* i = malloc(sizeof(ht_item));
    if (i == NULL){
        errormessage = memory_allocation_failed;
        return NULL;
    }
    i->key = _strdup(k);
    i->value = _strdup(v);
    return i;
}

static void ht_del_item(ht_item* i){
    free(i->key);
    free(i->value);
    free(i);
}

void ht_del_hash_table(ht_hash_table* ht){
    for (int i = 0; i < ht->size; ++i) {
        ht_item* item = ht->items[i];
        if (item != NULL){
            ht_del_item(item);
        }
    }
    free(ht->items);
    free(ht);
}

static int ht_hash(const char* s, const int a, const int m){
    long hash = 0;
    const int len_s = strlen(s);
    for (int i = 0; i < len_s; ++i) {
        hash += (long)pow(a, len_s - (i+1)) * s[i];
        hash = hash % m;
    }
    return (int)hash;
}

static int ht_get_hash(const char* s, const int num_buckets, const int attempt){
    const int HT_PRIME_1 = ht_random_prime();
    const int HT_PRIME_2 = ht_random_prime();
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

char* ht_search(ht_hash_table* ht, const char* key){
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while (item != NULL ) {
        if (item != &HT_DELETED_ITEM) {
        if (strcmp(item->key, key) == 0) {
            return item->value;
        }
    }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        ++i;
    }
    return NULL;
}

static void ht_delete(ht_hash_table* ht, const char* key){
    const int load = ht->count * 100 / ht->size;
    if(load < 10){
        ht_resize_down(ht);
    }
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while(item != NULL){
        if(item != &HT_DELETED_ITEM){
            if(strcmp(item->key, key) == 0){
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }
        index = ht_get_hash(key,ht->size,i);
        item = ht->items[index];
        ++i;
    }
    ht->count--;
}

static ht_hash_table* ht_new_sized(const int base_size){
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));
    if (ht == NULL){
        errormessage = memory_allocation_failed;
        return NULL;
    }
    ht->base_size = base_size;
    ht->size = next_prime(ht->base_size);
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
    if (ht->items == NULL){
        errormessage = memory_allocation_failed;
        return NULL;
    }
    return ht;
}

ht_hash_table* ht_new(){
    return ht_new_sized(HT_INITIAL_BASE_SIZE);
}

static void ht_resize(ht_hash_table* ht, const int base_size){
    if (base_size < HT_INITIAL_BASE_SIZE){
        return;
    }
    ht_hash_table* new_ht = ht_new_sized(base_size);
    for (int i = 0; i < ht->size; ++i) {
        ht_item* item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM){
            ht_insert(new_ht, item->key, item->value);
        }
    }
    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    ht_item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    ht_del_hash_table(new_ht);
}

static void ht_resize_up(ht_hash_table* ht){
    const int new_size = ht->base_size / 2;
    ht_resize(ht, new_size);
}

static  void ht_resize_down(ht_hash_table* ht){
    const int new_size = ht->base_size * 2;
    ht_resize(ht, new_size);
}

void ht_insert(ht_hash_table* ht, const char* key, const char* value){
    const int load = ht->count * 100 / ht->size;
    if (load > 70) {
        ht_resize_up(ht);
    }
    ht_item* item = ht_new_item(key,value);
    int index = ht_get_hash(item->key, ht->size, 0);
    ht_item* cur_item = ht->items[index];
    int i = 1;
    while (cur_item != NULL){
        if(cur_item != &HT_DELETED_ITEM){
            if(strcmp(cur_item->key, key)==0){
                ht_del_item(cur_item);
                ht->items[index] = item;
                return;
            }
        }
        index = ht_get_hash(item->key, ht->size, i);
        cur_item = ht->items[index];
        ++i;
    }
    ht->items[index] = item;
    ht->count++;
}