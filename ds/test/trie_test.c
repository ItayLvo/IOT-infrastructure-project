#include <stdio.h>	/* printf */

#include "trie.h"


int main()
{

	trie_t *trie = TrieCreate(8);
	unsigned int key_request1 = 155;
	unsigned int res = 0;
	unsigned int *result = &res;
	unsigned int key_request2 = 32;
	
	printf("before insert, trie size: %lu\n", TrieSize(trie));
	
	printf("insert status = %d\n", TrieInsert(trie, key_request1, result));
	
	printf("key recieved: %d\n", *(int *)result);
	
	printf("after insert, trie size: %lu\n", TrieSize(trie));
	
	TrieRemove(trie, key_request1);
	
	printf("after remove, trie size: %lu\n", TrieSize(trie));
	
	printf("insert status = %d\n", TrieInsert(trie, key_request1, result));
	
	printf("key recieved: %d\n", *(int *)result);
	
	printf("after insert, trie size: %lu\n", TrieSize(trie));
	
	
	/**********/
	
	printf("insert status = %d\n", TrieInsert(trie, key_request2, result));
	
	printf("key recieved: %d\n", *(int *)result);
	
	printf("after second insert, trie size: %lu\n", TrieSize(trie));
	
	
	TrieDestroy(trie);
	
	return 0;
}
