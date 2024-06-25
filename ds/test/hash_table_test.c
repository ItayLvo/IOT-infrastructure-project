#include <stdio.h>	/* printf */

#include "hash_table.h"
#include "dllist.h"	/* doubley linked list data structure and functions */


static size_t HashFunction(const void *key);
static int MatchInt(const void *item, const void *data_to_compare);


int main()
{
	int key1 = 5;
	char *data1= "itay";
	
	int key2 = 10;
	char *data2= "jorge";
	
	int key3 = 6;
	char *data3= "or";
	
	int key4 = 3;
	char *data4= "evelin";
	
	hash_table_t *table = HashTableCreate(HashFunction, MatchInt, 10);
	
	
	printf("Table size: %lu\n", HashTableSize(table));
	printf("Table IsEmpty: %d\n", HashTableIsEmpty(table));
	
	
	HashTableInsert(table, &key1, &data1);
	HashTableInsert(table, &key2, &data2);
	HashTableInsert(table, &key3, &data3);
	HashTableInsert(table, &key4, &data4);
	
	printf("Table size (after pushing 4 keys): %lu\n", HashTableSize(table));
	printf("Table IsEmpty: %d\n", HashTableIsEmpty(table));
	
	HashTableRemove(table, &key1);
	printf("Table size (after removing 1 key): %lu\n\n", HashTableSize(table));
	
	HashTablePrint(table);
	
	HashTableDestroy(table);
	
	return 0;
}






static int MatchInt(const void *item, const void *data_to_compare)
{
 	return (*(int *)item == *(int *)data_to_compare);
}


static size_t HashFunction(const void *key)
{
	return (*(int *)key % 5);
}


