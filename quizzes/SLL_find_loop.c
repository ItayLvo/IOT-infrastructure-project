#include <stddef.h>	/* size_t */
#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

typedef struct node
{
	int *data;
	struct node *next;
} node_t;



node_t *HasLoop(node_t *head);
node_t *UnLoop(node *intersection);

static void printList(node_t* head);
static int SizeOfList(node_t *node);

int HasLoop(node_t *head)
{
	node_t *fast_runner = head;
	node_t *slow_runner = head;
	
	while (fast_runner != NULL && slow_runner != NULL && fast_runner->next != NULL)
	{
		fast_runner = fast_runner->next->next;
		slow_runner = slow_runner->next;
		if (fast_runner == slow_runner)
		{
			return slow_runner;
		}
	}
	
	return NULL;
}

node_t *UnLoop(node *intersection)
{
	node_t *runner = intersection;
	while (runner->next != intersection)
	{
		runner = runner->next;
	}
	runner->next = NULL;
	
	return intersection;
}



int main()
{
	int x1 = 1, x2 = 2, x3 = 3, x4 = 4;
	
	node_t* node1 = (node_t*)malloc(sizeof(node_t));
	node_t* node2 = (node_t*)malloc(sizeof(node_t));
	node_t* node3 = (node_t*)malloc(sizeof(node_t));
	node_t* node4 = (node_t*)malloc(sizeof(node_t));
	node_t* head = NULL;
	
	node1->data = &x1;
	node2->data = &x2;
	node3->data = &x3;
	node4->data = &x4;
	
	head = node1;
	
	node1->next = node2;
	node2->next = node4;
	
	node3->next = node4;
	
	node4->next = NULL;
	/*
	node4->next = node1;
	
	*/
	/*
	printf("list: \n");
	printList(head);
	head = Flip(head);
	printf("\nReversed linked list \n");
	printList(head);
	printf("\n");
	*/
	
	printf("\n%d\n", *(int *)FindIntersection(head, node3)->data);
    
    return 0;
}





/* Function to print linked list */
static void printList(node_t* head)
{
	while (head != NULL)
	{
		printf("%d ", *(int *)(head->data));
		head = head->next;
	}
}


static int SizeOfList(node_t *node)
{
	int size = 0;
	while (node != NULL)
	{
		++size;
		node = node->next;
	}
	return size;
}

