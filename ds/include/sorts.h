#ifndef __SORTS_H__
#define __SORTS_H__

void InsertionSort(int *arr, int n);
void BubbleSort(int *arr, int n);
void SelectionSort(int *arr, int n);
void CountingSort(int *arr, int n);
void RadixSort(int *arr, int n);
void CountingSortPerDigit(int *arr, int n, int curr_digit);
int IterativeBinarySearch(int *arr, size_t size, int target);
int RecursiveBinarySearch(int *arr, size_t size, int target);
int MergeSort(int *arr_to_sort, size_t num_elements);
void QuickSort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*));

#endif	/*__SORTS_H__*/
