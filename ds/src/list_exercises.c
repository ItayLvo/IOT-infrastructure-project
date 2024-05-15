#include <stddef.h>	/* size_t */
#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

typedef struct node
{
	int *data;
	struct node *next;
} node_t;


node_t *Flip(node_t *head);
int HasLoop(node_t *head);
node_t *FindIntersection(node_t *head_1, node_t *head_2);
void push(node_t* head_ref, node_t *new_data);
void printList(node_t* head);
int SizeOfList(node_t *node);

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



/* reverse the order of a given list */
node_t *Flip(node_t *head)
{
	
	node_t *prev = NULL;
	node_t *curr = head;
	node_t *next = head->next;
	
	while (next != NULL)
	{
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	printf("asdasd %d ", *(int *)(prev->data));
	return prev;
}

/* checks if a list has a loop */
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
			return 1;
		}
	}
	
	return 0;
}


node_t *FindIntersection(node_t *head_1, node_t *head_2)
{
	size_t size1 = SizeOfList(head_1);
	size_t size2 = SizeOfList(head_2);
	int diff = 0;
	
	if (size1 > size2)
	{
		diff = size1 - size2;
		while (diff > 0)
		{
			head_1 = head_1->next;
			--diff;
		}
	}
	else if (size2 > size1)
	{
		diff = size2 - size1;
		while (diff > 0)
		{
			head_2 = head_2->next;
			--diff;
		}
	}
	
	while (head_1 != NULL && head_2 != NULL)
	{
		if (head_1 == head_2)
		{
			return head_1;
		}
		else
		{
			head_1 = head_1->next;
			head_2 = head_2->next;
		}
	}
	
	return NULL;
}


/* Function to print linked list */
void printList(node_t* head)
{
	while (head != NULL)
	{
		printf("%d ", *(int *)(head->data));
		head = head->next;
	}
}


int SizeOfList(node_t *node)
{
	int size = 0;
	while (node != NULL)
	{
		++size;
		node = node->next;
	}
	return size;
}

