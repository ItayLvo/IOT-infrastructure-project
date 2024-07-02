#ifndef __TRIE_H__
#define __TRIE_H__

typedef struct trie trie_t;

/*/**/
/*Description: Hash function that recieves a key and returns the index of the bucket it should be inserted into.*/
/**/*/
/*typedef size_t (*hash_func_t)(const void *key);*/

/*/**/
/*Description: Compare function between keys*/
/*Returns: 0 for equality, non-0 otherwise.*/
/**/*/
/*typedef int (*hash_cmp_func_t)(const void *key1, const void *key2);*/


/*
Descripion: Create a trie.
params: none
Return value: trie pointer, or NULL on failure
time complexity: O(1)
space complexity: O(1)
*/
trie_t *TrieCreate(void);

/*
Descripion: Destroy the trie.
Params: pointer to trie.
Return value: void
Time complexity: O(n)
Space complexity:O(1)
*/
void TrieDestroy(trie_t *trie);

/*
HashTableRemove
Descripion: Remove the data from the hash_table.
params: pointer to hash table, key to be removed
Return value: void
time complexity:  worst O(n) amortized O(1)
space complexity: O(1)
*/
void HashTableRemove(hash_table_t *table, const void *key);		/* ???? */

/*
Descripion: Insert a key into the trie.
params: pointer to hash table, void pointer to key, void pointer to data
Return value: int boolean value - 0 if succeeded, non-0 if failed
time complexity: worst O(n) amortized O(1)
space complexity: O(1)
*/
int TrieInsert(trie_t *trie, const void *key);

/*
HashTableSize
Descripion: Returns the amount of possible routes in a trie.
params: pointer to trie.
Return value: size_t, the amount of routes.
time complexity: O(n)
space complexity: O(1)
*/
size_t TrieSize(const trie_t *trie);

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




#endif /*__TRIE_H__*/
