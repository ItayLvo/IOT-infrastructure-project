#include <stdio.h>	/* printf */

void InsertionSort(int *arr, int n);
void BubbleSort(int arr[], int n);
void SelectionSort(int *arr, int n);

static void Swap(int *a, int *b);
static void PrintArray(int *arr, int n);


int main()
{
	int arr1[] = {12, 11, 13, 5, 6};
	int arr2[] = {12, 11, 13, 5, 6};
	int arr3[] = {12, 11, 13, 5, 6};
	int n = sizeof(arr1) / sizeof(arr1[0]);

	InsertionSort(arr1, n);
	PrintArray(arr1, n);

	BubbleSort(arr2, n);
	PrintArray(arr2, n);
	
	SelectionSort(arr3, n);
	PrintArray(arr3, n);

	return 0;
}



void SelectionSort(int *arr, int n)
{
	int i = 0, j = 0, min_index = 0;
    
	for (i = 0; i < n - 1; ++i)
	{
		min_index = i;
		for (j = i + 1; j < n; ++j)
		{
			if (arr[j] < arr[min_index])
			{
				min_index = j;
			}
		}
		Swap(&arr[min_index], &arr[i]);
	}
}


void BubbleSort(int arr[], int n)
{
	int i = 0, j = 0;
    
	for (i = 0; i < n - 1; ++i)
	{
		for (j = 0; j < n - i - 1; ++j)
		{
			if (arr[j] > arr[j + 1])
			{
				Swap(&arr[j], &arr[j + 1]);
			}
		}
	}
}


void InsertionSort(int *arr, int n)
{
	int i = 0, j = 0;
    
	for (i = 1; i < n; ++i)
	{
		j = i;
		while (j > 0 && arr[j] < arr[j - 1])
		{
			Swap(&arr[j], &arr[j - 1]);
			--j;
		}
	}
}




static void Swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}


static void PrintArray(int *arr, int n)
{
	int i = 0;
	
	for (i = 0; i < n; ++i)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
}


