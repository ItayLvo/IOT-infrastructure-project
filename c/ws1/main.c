#include <stdio.h>
#include "tentimesn.h"
#include "flip.h"
#include "swap.h"


long l = 8;
long *foo() { return &l; }
	
	
int main()
{
	int n = 3;
	int a=5, b=10;
	
	printf("10^n = 10^%d = %d\n\n", n, PowerTen(n));
	
	printf("flip: sent 1234, received %d \n\n", flip(1234));
	
	printf("before swap a = %d, b = %d. after swap: ", a, b);
	swap(&a, &b);
	printf("a = %d, b = %d\n", a, b);
	
	
	int x = 20;
	int y = 35;
	x = y++ + x++;
	y = ++y + ++x;
	printf("\n %d %d \n\n\n", x, y);
	
	*(foo()) = 5;
	printf("%ld\n", l);
	
	return 0;
}
