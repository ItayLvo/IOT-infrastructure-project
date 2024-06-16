#include <stdio.h>	/* printf */

void SortZeros(int *arr, int size);
static void Swap(int *a, int *b);

int main()
{
	int i = 0, arr[10] = {1,0,1,0,0,1,1,0,1,0};
	SortZeros(arr, 10);
	
	for (i = 0; i < 10; ++i)
	{
		printf("%d, ", arr[i]);
	}
	
	
	printf("\n");
    
    return 0;
}



void SortZeros(int *arr, int size)
{
	int ptr_zero = 0, ptr_one = 0;
	
	while (ptr_zero < size)
	{
		while (ptr_one < size && arr[ptr_one] != 1)
		{
			++ptr_one;
		}
		
		if (ptr_one > ptr_zero)
		{
			ptr_zero = ptr_one + 1;
		}
		
		while (ptr_zero < size && arr[ptr_zero] != 0)
		{
			++ptr_zero;
		}
		
		if (ptr_one < size && ptr_zero < size)
		{
			Swap(&arr[ptr_zero], &arr[ptr_one]);
		}
	}
}

static void Swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
