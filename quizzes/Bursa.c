#include <stdio.h>	/*printf*/

int Bursa(int *arr, int size, int *res);
void BursaV2(int *arr, int size, int *res);

int main()
{
	int array[] = {6, 12, 3, 5, 1, 4, 9, 2};
	
	/* result[0] = buy, result[1] = sell, result[2] = profit */
	int result[3] = {0, 0, 0};
	int size = sizeof(array) / sizeof(array[0]);
	BursaV2(array, size, result);
	
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

void BursaV2(int *arr, int size, int *res)
{
	int max_profit = 0;
	int tmp_profit = 0;
	int tmp_min_index = 0;
	int min = arr[0];
	int i = 1;
	
	for (i = 1; i < size; ++i)
	{
		if (arr[i] > min) 
		{
			tmp_profit = arr[i] - arr[tmp_min_index];
			if (tmp_profit > max_profit)
			{
				max_profit = tmp_profit;
				res[1] = i;
				res[0] = tmp_min_index;
			}
		}
		else if (arr[i] < min)
		{
			min = arr[i];
			tmp_min_index = i;
		}
	}
	res[2] = max_profit;
	return;
}
