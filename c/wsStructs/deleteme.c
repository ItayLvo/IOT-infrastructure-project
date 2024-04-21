#include <stdlib.h>
#include <stdio.h>

union numbers
{
	int i;
	float f;
	double d;
};

struct x
{
	int i;
	char c;
	double d;
	short s;
};

struct card
{
	unsigned int suit : 2;
	unsigned int face_value : 4;
};

int main()
{
	union numbers n1;
	struct x x1;
	struct card c1;
	printf("size of numbers: %ld\n", sizeof(n1));
	
	printf("size of numbers: %ld\n", sizeof(x1));
	
	printf("size of numbers: %ld\n", sizeof(c1));
	
	return 0;
}


