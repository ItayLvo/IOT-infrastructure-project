#include <stddef.h>	/* size_t */
#include <string.h>	/* memcpy */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */

#include "../include/singly_linked_list.h"

typedef struct node
{
	void *data;
	struct node* next;
	int is_dummy;
} node_t;


struct linked_list
{
	node_t *head; 
	node_t *tail;
};



