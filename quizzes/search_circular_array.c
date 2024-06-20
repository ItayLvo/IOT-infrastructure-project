#include <stdio.h>	/* printf */

#define SIZE 5

size_t BinarySearch(int *arr, size_t start, size_t end, int target);
size_t Search(int *arr, size_t size, int target);

int main()
{
	int target = 5;
	int arr[SIZE] = {5,4,1,2,3};
	size_t result = Search(arr, SIZE, target);
	
	printf("result: %lu\n", result);
    
	return 0;
}



size_t Search(int *arr, size_t size, int target)
{
	size_t result = BinarySearch(arr, 0, size-1, target);
	
	return result;
}

size_t BinarySearch(int *arr, size_t start, size_t end, int target)
{
	size_t mid = (start + end)/2;
	
	if (arr[mid] == target)
	{
		return mid;
	}
	
	if (target > arr[mid])
	{
		if (target > arr[end])
		{
			return BinarySearch(arr, start, mid - 1, target);
		}
		else
		{
			return BinarySearch(arr, mid + 1, end, target);
		}
	}
	else
	{
		if (target < arr[start])
		{
			return BinarySearch(arr, mid + 1, end, target);
		}
		else
		{
			return BinarySearch(arr, start, mid - 1, target);
		}
	}
}


