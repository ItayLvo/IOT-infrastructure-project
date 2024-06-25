#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */
#include <stdlib.h>	/* malloc */

#include "hash_table.h"
#include "dllist.h"	/* doubley linked list data structure and functions */



struct hash_table
{
	dll_t **buckets;
	hash_func_t hash_func;
	hash_cmp_func_t compare_func;
	size_t hash_table_size;
};


hash_table_t *HashTableCreate(hash_func_t hash_func, hash_cmp_func_t cmp_func, size_t hash_table_size)
{
	hash_table_t *table = (hash_table_t*)malloc(sizeof(hash_table_t));
	size_t i = 0;
	
	if (NULL == table)
	{
		return NULL;
	}
	
	table->buckets = (dll_t **)malloc(hash_table_size * sizeof(dll_t *));
	
	/* create a DLL for each "bucket" */
	for (i = 0; i < hash_table_size; ++i)
	{
		*(table->buckets + i) = DLListCreate();
		/* check if bucket successfully created. if not, free the table and all buckets allocated so far */
		if (NULL == *(table->buckets + i))
		{
			size_t j = 0;
			for (j = 0; j < i; ++j)
			{
				free(*(table->buckets + i));
			}
			free(table);
			return NULL;
		}
	}
	
	table->hash_func = hash_func;
	table->compare_func = cmp_func;
	table->hash_table_size = hash_table_size;
	
	return table;
}




void HashTableDestroy(hash_table_t *table)
{
	size_t i = 0;
	
	for (i = 0; i < table->hash_table_size; ++i)
	{
		DLListDestroy(*(table->buckets + i));
	}
	
	free(table->buckets);
	free(table);
}

