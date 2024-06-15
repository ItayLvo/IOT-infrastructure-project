#include <stdio.h>	/* printf */
#include <stdlib.h>	/* rand */
#include <time.h>	/* time() */

#define SIZE 5000

void InsertionSort(int *arr, int n);
void BubbleSort(int *arr, int n);
void SelectionSort(int *arr, int n);

static void Swap(int *a, int *b);
static void PrintArray(int *arr, int n);
static int Compare(const void *a, const void *b);


int main()
{
	int arr1[SIZE], arr2[SIZE], arr3[SIZE], arr4[SIZE];
	clock_t start;
	clock_t end;
	double sort_time = 0;
	int i = 0;
	srand(time(NULL));
    

	for (i = 0; i < SIZE; ++i)
	{
		arr1[i] = rand();
	}
    
	for (i = 0; i < SIZE; ++i)
	{
		arr2[i] = arr1[i];
		arr3[i] = arr1[i];
		arr4[i] = arr1[i];
	}


	start = clock();
	SelectionSort(arr1, SIZE);
	end = clock();
	sort_time = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("SelectionSort took %f seconds\n", sort_time);
	
	
	start = clock();
	BubbleSort(arr2, SIZE);
	end = clock();
	sort_time = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("BubbleSort took %f seconds\n", sort_time);
	
	
	start = clock();
	InsertionSort(arr3, SIZE);
	end = clock();
	sort_time = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("InsertionSort took %f seconds\n", sort_time);
    

	start = clock();
	qsort(arr4, SIZE, sizeof(int), Compare);
	end = clock();
	sort_time = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("qsort took %f seconds\n", sort_time);
	

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


void BubbleSort(int *arr, int n)
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


static int Compare(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
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


