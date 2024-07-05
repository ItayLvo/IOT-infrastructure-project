#include <stdio.h>	/* printf */

#include "trie.h"


int main()
{

	trie_t *trie = TrieCreate(8);
	unsigned int key_request = 155;
	unsigned int res = 0;
	unsigned int *result = &res;
	
	printf("insert status = %d\n", TrieInsert(trie, key_request, result));
	
	printf("key recieved: %d\n", *(int *)result);
	
	printf("insert status = %d\n", TrieInsert(trie, key_request, result));
	
	printf("key recieved: %d\n", *(int *)result);
	
	TrieDestroy(trie);
	
	return 0;
}
