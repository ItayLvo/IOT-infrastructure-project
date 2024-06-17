#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "recursion.h"


int TestFlipList();

int main()
{
	printf("%d\n", Fibonacci(15));
	
	TestFlipList();
	
	return 0;
}



int TestFlipList()
{
	node_t *head = NULL;

	int a = 1, b = 2, c = 3, d = 4, e = 5;
	AppendNode(&head, &a);
	AppendNode(&head, &b);
	AppendNode(&head, &c);
	AppendNode(&head, &d);
	AppendNode(&head, &e);

	printf("Original list: ");
	PrintList(head, PrintInt);

	head = FlipList(head);

	printf("Reversed list: ");
	PrintList(head, PrintInt);

	FreeList(head);

	return 0;
}

