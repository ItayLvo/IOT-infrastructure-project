#include <stdio.h>	/* printf */
#include <time.h>	/* time() */
#include <stdlib.h>	/* rand, srand, qsort */

#include "sorts.h"

#define SIZE 10
#define MIN_7_DIGIT 1000000
#define MAX_7_DIGIT 9999999

static void PrintArray(int *arr, int n);
static int Compare(const void *a, const void *b);
static void GenerateRandomArray(int *arr, size_t len);

static void TestMergeSort();
static void TestBinarySearch();
static void TestQuickSort();


int main()
{
	int arr1[SIZE], arr2[SIZE], arr3[SIZE], arr4[SIZE], arr7[SIZE];
	clock_t start;
	clock_t end;
	double sort_time = 0;
	int i = 0;
	
	int arr5[9] = {4,2,7,5,2,3,6,6,10};
	int arr6[9] = {4,2,7,5,2,3,6,6,10};
	
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
	
	for (i = 0; i < SIZE; ++i)
	{
		arr7[i] = MIN_7_DIGIT + rand() % (MAX_7_DIGIT - MIN_7_DIGIT + 1);
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
	
	
	printf("before counting sort: \n");
	PrintArray(arr5, 9);
	printf("after counting sort: \n");
	CountingSort(arr5, 9);
	PrintArray(arr5, 9);
	
	printf("before qsort: \n");
	PrintArray(arr6, 9);
	printf("after qsort: \n");
	qsort(arr6, 9, sizeof(int), Compare);
	PrintArray(arr6, 9);
	
	printf("\n");
	
	printf("before radix sort: \n");
	PrintArray(arr7, SIZE);
	printf("after radix sort: \n");
	RadixSort(arr7, SIZE);
	PrintArray(arr7, SIZE);
	
	
	TestBinarySearch();
	TestMergeSort();
	TestQuickSort();
	
	
	return 0;
}


static void TestBinarySearch()
{
	int arr1[9] = {-2, 4, 6, 7, 8, 9, 9, 11, 15};
	int arr2[9] = {-2, 4, 6, 7, 8, 9, 9, 11, 15};
	
	
	
	printf("Iterative binary search test:\nindex of target (should be 7): %d\n", IterativeBinarySearch(arr1, 9, 11));
	printf("Recursive binary search test:\nindex of target (should be 0): %d\n\n", RecursiveBinarySearch(arr2, 9, -2));
	
}



static void TestQuickSort()
{
	int arr1[5000];
	int arr2[5000];
	
	size_t i = 0;
	clock_t start_time = 0;
	clock_t end_time = 0;
	size_t total_time = 0;

	GenerateRandomArray(arr1, 5000);
	GenerateRandomArray(arr2, 5000);
	
	
	start_time = clock();
	
	QuickSort(arr1, 5000, sizeof(int), Compare);
	
	end_time = clock();
	total_time = end_time - start_time;
	printf("time for manual quicksort: %ld\n", total_time);
	
	
	start_time = clock();
	qsort(arr2, 5000, sizeof(int), Compare);
	end_time= clock();
	total_time = end_time - start_time;
	printf("time for system qsort: %ld\n", total_time);
	
	for (i = 0; i < 5000; ++i)
	{
		if (arr1[i] != arr2[i])
		{
			printf("Quick Sort failed at index: %ld\n", i);
			break;
		}
	}
	
	
	printf("\nEnd of quick sort test.\n");
}

	


static void TestMergeSort()
{
	int arr1[9] = {15, 4, 6, -4, 11, 9, 8, 11, 3};
	size_t i = 0;
	
	MergeSort(arr1, 9);
	
	for (i = 0; i < 9; ++i)
	{
		printf("[%d] ", arr1[i]);
	}
	printf("\nEnd of merge sort test.\n");
}



static int Compare(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
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





static void GenerateRandomArray(int *arr, size_t len)
{
	size_t i = 0;
	
	srand(time(NULL));
	for (i = 0; i < len; ++i)
	{
		arr[i] = rand() % 201 - 100;
	}
}

