#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

typedef struct hash_table hash_table_t;

/*
Description: Hash function that recieves a key and returns the index of the bucket it should be inserted into.
*/
typedef size_t (*hash_func_t)(const void *key);

/*
Description: Action function that recieves the data of an element and params and does some action on the data.
*/
typedef int (*hash_action_func_t)(void *data, void *params);

/*
Description: Compare function between keys
Returns: 0 for equality, non-0 otherwise.
*/
typedef int (*hash_cmp_func_t)(const void *key1, const void *key2);


/*
HashTableCreate
Descripion: Create the hash table.
params: key size in bytes, value size in bytes, number of buckets to store hashed values
Return value: hash table pointer or NULL on failure
time complexity: O(n) (n = hash table size)?
space complexity: O(n) (n = hash_table_size)?
*/
hash_table_t *HashTableCreate(hash_func_t hash_func, hash_cmp_func_t cmp_func, size_t hash_table_size);

/*
HashTableDestroy
Descripion: Destroy the hash table.
Params: pointer to hash table
Return value: void
Time complexity: O(n)
Space complexity:O(1)
*/
void HashTableDestroy(hash_table_t *table);

/*
HashTableRemove
Descripion: Remove the data from the hash_table.
params: pointer to hash table, key to be removed
Return value: void
time complexity:  worst O(n) amortized O(1)
space complexity: O(1)
*/
void HashTableRemove(hash_table_t *table, const void *key);

/*
int HashTableInsert
Descripion: Insert the the data hash table.
params: pointer to hash table, void pointer to key, void pointer to data
Return value: int boolean value - 0 if succeeded, non-0 if failed
time complexity: worst O(n) amortized O(1)
space complexity: O(1)
*/
int HashTableInsert(hash_table_t *table, const void *key, void *data);

/*
HashTableSize
Descripion: Returns the size of the table.
params: pointer to hash table.
Return value: size_t, the amount of elements.
time complexity: O(n)
space complexity: O(1)
*/
size_t HashTableSize(const hash_table_t *table);

/*
HashTableIsEmpty
Descripion: Check if the table is empty. (do not use size func but return if find 1 element)
params: pointer to hash table.
Return value: bool. 1 - True, 0 - False.
time complexity: O(n) (n = number_of_buckets)
space complexity: O(1)
*/
int HashTableIsEmpty(const hash_table_t *table);

/*
HashTableFind
Descripion: Find an element using its key.
Input: hash table, and key to search for
return: value if found NULL if Failed 
time complexity: worst O(n) amortized O(1)
space complexity: O(1)
*/
void *HashTableFind(const hash_table_t *table, const void *key);


/*
HashTableForEach
Descripion: Apply hash_action_func_t func for each element in the table. Stops when done or when action function return something other than 0(Failure).
Input: hash table, an action func and params
return: last status from action function
time complexity: O(n)
space complexity: O(1)
*/
int HashTableForEach(hash_table_t *table, hash_action_func_t action, void *params);



/****************************************Advanced***********************************************/


/* HashTableLoad */

double HashTableLoad(const hash_table_t *table);

/* HashTableStandartDeviasion */

double HashTableStandardDeviation(const hash_table_t *table);


#endif /*__HASHTABLE_H__*/
