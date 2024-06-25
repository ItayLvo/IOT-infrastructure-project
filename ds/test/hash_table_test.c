#include <stdio.h>	/* printf */

#include "hash_table.h"
#include "dllist.h"	/* doubley linked list data structure and functions */


int main()
{
	hash_table_t *table = HashTableCreate(NULL, NULL, 5);
	HashTableDestroy(table);
	
	return 0;
}
