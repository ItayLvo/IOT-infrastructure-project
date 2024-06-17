#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "recursion.h"


static int TestFlipList();

int main()
{
	char *string = "12345678";
	char *string2 = "12345678";
	char *dest = malloc(sizeof(char) * (Strlen(string) + 1) * 2);
	
	
	/* fibonacci test */
	printf("Fibonacci:\nExpected result is 610, result is: %d\n\n", Fibonacci(15));
	
	/* FlipList test */
	TestFlipList();
	printf("\n");
	
	/* Strlen test */
	printf("expected Strlen result is 8, result: %ld\n\n", Strlen(string));
	
	printf("%d\n\n", Strcmp(string, string2));
	
	printf("%s\n", dest);
	dest = Strcpy(dest, string);
	printf("%s\n", dest);
	
	Strcat(dest, string);
	printf("%s\n", dest);
	
	/* need more complex testing... */
	printf("%s\n", Strstr(dest, string));
	
	free(dest);
	
	return 0;
}



static int TestFlipList()
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

