#include <stdio.h>	/*printf*/

int Bursa(int *arr, int size, int *res);

int main()
{
	int array[] = {6, 12, 3, 5, 1, 4, 9, 2};
	
	/* result[0] = buy, result[1] = sell, result[2] = profit */
	int result[3] = {0, 0, 0};
	int size = sizeof(array) / sizeof(array[0]);
	Bursa(array, size, result);
	
	printf("%d %d %d\n", result[0], result[1], result[2]);
	return 0;
}



int Bursa(int *arr, int size, int *res)
{
	int buy = arr[0];
	int profit = 0;
	int i = 1;
	
	for (i = 1; i < size; ++i)
	{
		if (arr[i] < buy) 
		{
			buy = arr[i];
			res[0] = i;
		}
		else if (arr[i] - buy > profit)
		{
			profit = arr[i] - buy;
			res[1] = i;
		}
	}
	res[2] = profit;
	return profit;
}
