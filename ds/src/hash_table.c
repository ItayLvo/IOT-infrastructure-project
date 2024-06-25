#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */
#include <stdlib.h>	/* malloc */

#include <stdio.h>	/* printf ---------------------------------------------- remove me! */

#include "hash_table.h"
#include "dllist.h"	/* doubley linked list data structure and functions */


struct hash_table
{
	dll_t **buckets;
	hash_func_t hash_func;
	hash_cmp_func_t compare_func;
	size_t hash_table_size;
};


typedef struct
{
	const void *key;
	void *data;
} element_t;


static element_t *CreateHashTableElement(const void *key, void *data);
static dll_iterator_t HashTableFindElementInBucket(hash_table_t *table, const void *key);

static int HashTablePrintHelper(void *data, void *params);


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
	dll_t *current_bucket = NULL;
	dll_iterator_t list_runner = {0};
	element_t *current_element = NULL;
	
	
	for (i = 0; i < table->hash_table_size; ++i)
	{
		current_bucket = *((table->buckets) + i);
		list_runner = DLListBegin(current_bucket);
		
		while (!DLListIsEqualIter(list_runner, DLListEnd(current_bucket)))
		{
			current_element = (element_t *)DLListGetData(list_runner);
			free(current_element);
			list_runner = DLListNext(list_runner);
		}
		
		DLListDestroy(current_bucket);
	}
	
	free(table->buckets);
	free(table);
}



int HashTableInsert(hash_table_t *table, const void *key, void *data)
{
	size_t hash_result = (table->hash_func)(key);
	dll_t *current_bucket = (table->buckets)[hash_result];
	dll_iterator_t insert_status;
	
	/* create element struct to wrap key-data pair */
	element_t *new_element = CreateHashTableElement(key, data);
	if (NULL == new_element)
	{
		return 1;
	}
	
	/* insert the element (key-data pair) to the relevant bucket */
	insert_status = DLListInsert(current_bucket, DLListBegin(current_bucket), new_element);
	
	/* check DLL insert validity */
	if (DLListIsEqualIter(insert_status, DLListEnd(current_bucket)))
	{
		free(new_element);
		return 1;
	}
	
	return 0;
}




void HashTableRemove(hash_table_t *table, const void *key)
{
	element_t *element_to_remove = NULL;
	dll_iterator_t iterator_to_remove = HashTableFindElementInBucket(table, key);
	
	if (NULL != iterator_to_remove)
	{
		element_to_remove = DLListGetData(iterator_to_remove);
		free(element_to_remove);
		DLListRemove(iterator_to_remove);
	}
}



void *HashTableFind(const hash_table_t *table, const void *key)
{
	dll_iterator_t iterator_to_find = HashTableFindElementInBucket((hash_table_t *)table, key);
	element_t *element_to_find = NULL;
	void *data_to_find = NULL;
	
	if (NULL == iterator_to_find)
	{
		return NULL;
	}
	
	element_to_find = DLListGetData(iterator_to_find);
	data_to_find = element_to_find->data;
	
	HashTableRemove((hash_table_t *)table, key);
	if (HashTableInsert((hash_table_t *)table, key, data_to_find))
	{
		return NULL;
	}
	
	return data_to_find;
}



size_t HashTableSize(const hash_table_t *table)
{
	size_t i = 0;
	size_t size_sum = 0;
	dll_t *current_bucket = NULL;
	
	for (i = 0; i < table->hash_table_size; ++i)
	{
		current_bucket = *(table->buckets + i);
		size_sum += DLListCount(current_bucket);
	}
	
	return size_sum;
}


int HashTableIsEmpty(const hash_table_t *table)	/* TODO: code resuse with foreach */
{
	size_t i = 0;
	dll_t *current_bucket = NULL;
	
	for (i = 0; i < table->hash_table_size; ++i)
	{
		current_bucket = *(table->buckets + i);
		if (!DLListIsEmpty(current_bucket))
		{
			return 0;
		}
	}
	
	return 1;
}



int HashTableForEach(hash_table_t *table, hash_action_func_t action_func, void *params)
{
	size_t i = 0;
	dll_t *current_bucket = NULL;
	dll_iterator_t current_element;
	
	for (i = 0; i < table->hash_table_size; ++i)
	{
		current_bucket = *(table->buckets + i);
		current_element = DLListForeach(DLListBegin(current_bucket), DLListEnd(current_bucket), params, action_func);
		
		if (!DLListIsEqualIter(current_element, DLListEnd(current_bucket)))
		{
			return 1;
		}
	}
	
	return 0;
}



static dll_iterator_t HashTableFindElementInBucket(hash_table_t *table, const void *key)
{
	size_t hash_result = (table->hash_func)(key);
	dll_t *current_bucket = (table->buckets)[hash_result];
	
	hash_cmp_func_t client_compare_func = table->compare_func;
	int match_result = 0;
	dll_iterator_t list_runner = DLListBegin(current_bucket);
	element_t *current_element = NULL;
	
	while (!DLListIsEqualIter(list_runner, DLListEnd(current_bucket)))
	{
		current_element = (element_t *)DLListGetData(list_runner);
		match_result = client_compare_func(current_element->key, key);
		if (match_result)
		{
			return list_runner;
		}
		
		list_runner = DLListNext(list_runner);
	}
	
	return NULL;
}



static element_t *CreateHashTableElement(const void *key, void *data)
{
	element_t *element = (element_t *)malloc(sizeof(element_t));
	if (NULL == element)
	{
		return NULL;
	}
	
	element->key = key;
	element->data = data;
	
	return element;
}




void HashTablePrint(hash_table_t *table)
{
	HashTableForEach(table, HashTablePrintHelper, NULL);
}
static int HashTablePrintHelper(void *data, void *params)
{
	element_t *current_element = (element_t *)data;
	int key = *(int *)current_element->key;
	char *string = *(char **)current_element->data;
	
	(void)params;
	
	printf("Key = %d\t\tData = %s\n", key, string);
	
	return 0;
}



