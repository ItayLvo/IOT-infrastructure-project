#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b);
void swapt(size_t *a, size_t *b);
void swaptp(size_t **ap, size_t **bp);
int PowerTen(int n);
int *CopyArray(int *arr, int size);
int flip(int n);
	
	
int main()
{
	int n = 3;
	int a = 5, b = 10;
	
	printf("10^n = 10^%d = %d\n\n", n, PowerTen(n));
	
	printf("flip: sent 1234, received %d \n\n", flip(1234));
	
	printf("before swap a = %d, b = %d. after swap: ", a, b);
	swap(&a, &b);
	printf("a = %d, b = %d\n", a, b);
	
	/* testing some expressions */
	int x = 20;
	int y = 35;
	x = y++ + x++;
	y = ++y + ++x;
	printf("\n %d %d \n\n\n", x, y);
	
	
	return 0;
}


int *CopyArray(int *arr, int size)
{
	int i = 0;
	int *newarr = malloc(size * sizeof(arr[0]));
	
	if (newarr == NULL)
	{
		return NULL;
	}
		
	for (i = 0; i < size; ++i)
	{
		newarr[i] = arr[i];
	}
	
	return newarr;
}

int flip(int n)
{
	int result = 0;
	
	while (n != 0)
	{
		result = result * 10;
		result += n % 10;
		n = n / 10;
	}
	
	return result;
}

void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
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


int PowerTen(int n)
{
	int result = 1;
	int i = 0;
	
	for (i = 0; i < n; ++i)
	{
		result = result * 10;
	}
	
	return result;
}


/*
tests to test swap, swapt, etc... methods listed above

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
*/




/*

Exercises - C Expressions worksheet


a.
Expression: a;
Type: int(l-value)
value: the value of a (initizliard or assigned)
side effect: none


b.
Expression: foo;
type: function
value: r-value
side effect: none


c.
Expression: &foo;
type: function pointer
value: r-value (address of a function is not a modifiable l-value, it's simply a value that represents the memory address of the function's entry point).
side effeft: none


d.
Expression: foo();
type: int r-value
value: the outcome of the function (returned value)
side effets: the side effects of calling the function


e.
Expression: a+ foo();
type: int(non l-value)
value - a + outcome of foo();
side effect: same as question d

f.
expression: &a
type: int*
value: l-value of the type pointer to by the pointer (int)
side effect: accessing illegal address may cause a crash
**
**
**
*/

/*
#include <stdio.h>
int main()
{
char st[] = {0x22, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x21,  0x22, '\0'};
printf("%s\n", st);
}
*/
