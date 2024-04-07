#include "swapsizet.h"
#include <stdio.h>
#include <stddef.h>

int main()
{

	// swapping size_t
	size_t a = sizeof(int);
	size_t b = sizeof(long long);
	printf("value of a = %zu, value of b = %zu \n", a, b);
	swapt(&a, &b);
	printf("performed swapt\n");
	printf("a = %zu, b = %zu \n\n", a, b);
	
	
	
	// swapping size_t pointers
	size_t *ap = &a;
	size_t *bp = &b;
	printf("address of a = %p, address b = %p \n", (void *)ap, (void *)bp);
	printf("value of a = %zu, value of b = %zu \n\n", *ap, *bp);
	
	swaptp(&ap, &bp);
	
	printf("address of a = %p, address b = %p \n", (void *)ap, (void *)bp);
	printf("value of a = %zu, value of b = %zu \n\n", *ap, *bp);
	
	
	// swapping size_t pointers again with swapt2
	
	swaptp(&ap, &bp);
	
	printf("address of a = %p, address b = %p \n", (void *)ap, (void *)bp);
	printf("value of a = %zu, value of b = %zu \n\n", *ap, *bp);
}



void swapt(size_t *a, size_t *b)
{
	// swapping size_t, with size_t pointers
	size_t tmp = *a;
	*a = *b;
	*b = tmp;
}


void swaptp(size_t **ap, size_t **bp)
{
	// swapping size_t pointers, with size_t pointers to pointers
	size_t *tmp = *ap;
	*ap = *bp;
	*bp = tmp;
}


void swaptp2(size_t **ap, size_t **bp)
{
	swapt(*ap, *bp);
}
