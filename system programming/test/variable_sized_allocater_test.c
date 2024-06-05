#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "variable_size_allocator.h"	/* fixed_size_allocator_t and FSA functions */

static void TestFlow2(void);

int main()
{
	
	long *px = malloc(128);
	long *p_test1 = NULL;
	long *p_test2 = NULL;
	long *p_test3 = NULL;
	long *tmp_adrs = NULL;
	vsa_t *allocator;
	
	printf("px: %p\n", px);
	
	allocator = VSAInitialize(px, 128);
	printf("allocator: %p\n", allocator);
	
	p_test1 = VSAAlloc(allocator, 80);
	printf("p_test1: %p\n", p_test1);
	
	p_test2 = VSAAlloc(allocator, 7);
	printf("p_test2: %p\n", p_test2);
	
	VSAFree(p_test2); 
	
	VSAFree(p_test1);
	
	
	p_test3 = VSAAlloc(allocator, 30);
	printf("p_test3: %p\n", p_test3);
	
	
	
	VSAFree(p_test3); 
	
	printf(" %ld\n", *(p_test1 - 1));
	printf(" %ld\n", *(p_test2 - 1));
	printf(" %ld\n\n\n", *(p_test3 - 1));
	
	
	free(px);
	
	TestFlow2();
	
	return 0;
}


static void TestFlow2(void)
{

	long *b1 = NULL;
	long *b2 = NULL;
	long *b3 = NULL;
	
	long *ptr = (long *)malloc(250);
	vsa_t *vsa = VSAInitialize(ptr, 250);
	
	b1 = (long *)VSAAlloc(vsa, 40);
	b2 = (long *)VSAAlloc(vsa, 80);
	b3 = (long *)VSAAlloc(vsa, 48);
	
	printf(" %ld\n", *(b1 - 1));
	printf(" %ld\n", *(b2 - 1));
	printf(" %ld\n", *(b3 - 1));
	
	VSAFree(b1);
	VSAFree(b2);
	
	printf(" %ld\n", *(b1 - 1));
	printf(" %ld\n", *(b2 - 1));
	printf(" %ld\n\n", *(b3 - 1));
	
	
	printf(" %p\n", (b1 - 1));
	printf(" %p\n", (long *)vsa + 1);
	
	b2 = (long *)VSAAlloc(vsa, 104);
	printf(" %ld\n", *(b2 - 1));
	printf(" %p\n", (b2 - 1));
	
	
	
	if(b2 - 1 != (long *)vsa + 1)
	{
		printf("VSAAlloc 2 - 1 failed\n");
		return;
	}
	
	(void)b3;
	printf("TestFlow2 passed\n");
	free(ptr);
	
}

