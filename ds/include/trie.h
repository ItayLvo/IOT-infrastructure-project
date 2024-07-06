#ifndef __TRIE_H__
#define __TRIE_H__

#include <stddef.h>		/* size_t */

typedef struct trie trie_t;


typedef enum trie_status
{
  TRIE_SUCCESS,	
  TRIE_MEMORY_FALIURE,
  TRIE_FULL,
  TRIE_BAD_REQUEST,
  TRIE_DOUBLE_FREE
} e_trie_status;


trie_t *TrieCreate(size_t height);

void TrieDestroy(trie_t *trie);

e_trie_status TrieInsert(trie_t *trie, const unsigned int requested_key, unsigned int *result_key);

void TrieRemove(trie_t *trie, unsigned int key);

size_t TrieSize(const trie_t *trie);


#endif /*__TRIE_H__*/
