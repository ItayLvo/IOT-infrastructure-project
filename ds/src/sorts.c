#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */
#include <time.h>	/* time() */

#define SIZE 10
#define MIN_7_DIGIT 1000000
#define MAX_7_DIGIT 9999999


void InsertionSort(int *arr, int n);
void BubbleSort(int *arr, int n);
void SelectionSort(int *arr, int n);
void CountingSort(int *arr, int n);
void RadixSort(int *arr, int n);
void CountingSortPerDigit(int *arr, int n, int curr_digit);

static void Swap(int *a, int *b);
static int FindMaxInArray(int *arr, int size);
static int FindMinInArray(int *arr, int size);
static int FindMaxInArrayPerDigit(int *arr, int n, int digit);
static int RecursiveBinarySearchHelper(int *arr, size_t start, size_t end, int target);
static int MergeSortRerucrsive(int *arr, size_t start, size_t end);
static void Merge(int *arr, size_t start, size_t mid, size_t end);

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

void CountingSort(int *arr, int n)
{
	int i = 0, k = 0;
	int max_element = FindMaxInArray(arr, n);
	int min_element = FindMinInArray(arr, n);
	int range_of_values = max_element - min_element;
	int *count_arr = calloc(range_of_values + 1, sizeof(int));
	
	for (i = 0; i < n; ++i)
	{
		++count_arr[arr[i]-min_element];
	}
	
	for (i = 0; i < range_of_values + 1; ++i)
	{
		int curr_number = i + min_element;
		if (count_arr[i] != 0)
		{
			while (count_arr[i] > 0)
			{
				arr[k] = curr_number;
				++k;
				--count_arr[i];
			}
		}
	}
	
	free(count_arr);
}


void RadixSort(int *arr, int n)
{
	int max_element = FindMaxInArray(arr, n);
	int curr_digit = 1;
	
	for (curr_digit = 1; max_element / curr_digit > 0; curr_digit *= 10)
	{
		CountingSortPerDigit(arr, n, curr_digit);
	}
}


void CountingSortPerDigit(int *arr, int n, int curr_digit)
{
	int *output = calloc(n + 1, sizeof(int));
	int max_element = FindMaxInArrayPerDigit(arr, n, curr_digit);
	int *count_arr = calloc(max_element + 1, sizeof(int));
	int i = 0;

	/* set up LUT */
	for (i = 0; i < n; ++i)
	{
		++count_arr[(arr[i] / curr_digit) % 10];
	}

	
	for (i = 1; i < 10; ++i)
	{
		count_arr[i] += count_arr[i - 1];
	}

	for (i = n - 1; i >= 0; --i)
	{
		output[count_arr[(arr[i] / curr_digit) % 10] - 1] = arr[i];
		--count_arr[(arr[i] / curr_digit) % 10];
	}

	for (i = 0; i < n; i++)
	{
		arr[i] = output[i];
	}
	
	free(output);
	free(count_arr);
}



int IterativeBinarySearch(int *arr, size_t size, int target)
{
	int start = 0;
	int end = size - 1;
	int mid = (start + end) / 2;
	
	
	while (start <= end)
	{
		if (target == arr[mid])
		{
			return mid;
		}
		else if (target < arr[mid])
		{
			end = mid - 1;
		}
		else	 /* (target > arr[mid]) */
		{
			start = mid + 1;
		}
		
		mid = (start + end) / 2;
	}
	
	return -1;
}


int RecursiveBinarySearch(int *arr, size_t size, int target)
{
	return RecursiveBinarySearchHelper(arr, 0, size-1, target);
}


static int RecursiveBinarySearchHelper(int *arr, size_t start, size_t end, int target)
{
	size_t mid = (start + end) / 2;
	
	if (arr[mid] == target)
	{
		return mid;
	}
	
	if (start <= end)
	{
		if (target < arr[mid])
		{
			return RecursiveBinarySearchHelper(arr, start, mid-1, target);
		}
		else	 /* (target > arr[mid]) */
		{
			return RecursiveBinarySearchHelper(arr, mid + 1, end, target);
		}
	}
	
	return -1;
}


int MergeSort(int *arr_to_sort, size_t num_elements)
{
	return MergeSortRerucrsive(arr_to_sort, 0, num_elements - 1);
}


static int MergeSortRerucrsive(int *arr, size_t start, size_t end)
{
	size_t mid = 0;
	
	if (start < end)
	{
		mid = start + (end - start) / 2;
		
		MergeSortRerucrsive(arr, start, mid);
		MergeSortRerucrsive(arr, mid + 1, end);
		
		Merge(arr, start, mid, end);
	}
	
	return 0;
}



static void Merge(int *arr, size_t start, size_t mid, size_t end)
{
	size_t len1 = mid - start + 1;
	size_t len2 = end - mid;
	
	int arr1[len1];
	int arr2[len2];
	
	size_t i = 0, j = 0, k = 0;

	/* init array of first half */
	for (i = 0; i < len1; ++i)
	{
		arr1[i] = arr[start + i];
	}
	
	/* init array of second half */
	for (i = 0; i < len2; ++i)
	{
		arr2[i] = arr[mid + i + 1];
	}
	
	/* merge the two arrays into the original arr, in a sorted order */
	i = 0;
	j = 0;
	k = start;
	while (i < len1 && j < len2)
	{
		if (arr1[i] <= arr2[j])
		{
			arr[k] = arr1[i];
			++i;
		}
		else
		{
			arr[k] = arr2[j];
			++j;
		}
		
		++k;
	}
	
	/* copy the remaining elements of arr1 */
	while (i < len1)
	{
		arr[k] = arr1[i];
		++i;
		++k;
	}

	/* copy the remaining elements of arr2 */
	while (j < len2)
	{
		arr[k] = arr2[j];
		++j;
		++k;
	}
	
}


/****** static helper functions ******/


static void Swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}


static int FindMaxInArray(int *arr, int size)
{
	int i = 0;
	int max_val = arr[i];
	for (i = 1; i < size; ++i)
	{
		if (arr[i] > max_val)
		{
			max_val = arr[i];
		}
	}
	
	return max_val;
}

static int FindMinInArray(int *arr, int size)
{
	int i = 0;
	int min_val = arr[i];
	for (i = 1; i < size; ++i)
	{
		if (arr[i] < min_val)
		{
			min_val = arr[i];
		}
	}
	
	return min_val;
}



static int FindMaxInArrayPerDigit(int *arr, int n, int digit)
{
	int max_element = (arr[0] / digit) % 10;
	int i = 1;
	
	for (i = 1; i < n; i++)
	{
		if (((arr[i] / digit) % 10) > max_element)
		max_element = arr[i];
	}

	return max_element;
}
