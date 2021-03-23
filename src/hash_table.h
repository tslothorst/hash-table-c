#ifndef HASH_TABLE_C_HASH_TABLE_H
#define HASH_TABLE_C_HASH_TABLE_H

typedef struct {
    char* key;
    char* value;
} ht_item;

typedef struct {
    int size;
    int count;
    int base_size;
    ht_item** items;
} ht_hash_table;

typedef enum { memory_allocation_failed  } error;

ht_hash_table* ht_new();
static ht_item* ht_new_item(const char* k, const char* v);
static void ht_del_item(ht_item* i);
void ht_del_hash_table(ht_hash_table* ht);

#endif //HASH_TABLE_C_HASH_TABLE_H
