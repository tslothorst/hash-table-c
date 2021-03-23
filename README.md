# Hash table implemented in C
My implementation of a hash table in C. This hash table implements double hashing to avoid collissions. 


## Usage
The api makes the following functions available:
```c
ht_hash_table* ht_new();
void ht_del_hash_table(ht_hash_table* ht);
void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* h, const char* key);
```

`ht_new` will create a new empty hash table. 
`ht_del_hash_table` will delete the hash table. 

`ht_insert` will insert a new item into the selected hash table.
`ht_search` will search for a key in the hash table and return it's value.
`ht_delete` will delete a specified item from the hash table and replace it with a sentinel value so as to preserve the collission chain. 

### Sources
Based on [a tutorial](https://github.com/jamesroutley/write-a-hash-table) by James Routley.