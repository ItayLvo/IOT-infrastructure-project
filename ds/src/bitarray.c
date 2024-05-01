#include <stdio.h>	/* printf */


typedef struct bitarray
{
    long data;
} bitarray;


int main()
{
	
	return 0;
}

void set_all(bitarray *bitarr)
{
	bitarr->data = 0xFFFFFFFFFFFFFFFF;
}

void reset_all(bitarray *bitarr)
{
	bitarr->data = 0;
}

void set_bit(bitarray *bitarr, size_t index)
{
	size_t mask = 1;
	mask <<= index;
	bitarr->data |= mask;
}


