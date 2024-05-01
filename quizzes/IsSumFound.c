#include <stdio.h>

int IsSomeFound(int *array, int size, int sum)
{
	int left = 0;
	int right = size - 1;
	int current_sum = 0;
	while (left < right)
	{
		current_sum = array[left] + array[right];

		if (current_sum == sum)
		{
			return 1;
		}
		else if (current_sum < sum)
		{
			left++;
		}
		else
		{
			right--;
		}
	}

	return 0;
}

int main()
{
	int array[] = {2, 4, 6, 23, 34, 44, 78, 99, 101};
	int size = sizeof(array) / sizeof(array[0]);
	printf("%d\n", IsSomeFound(array, size, 57));
	return 0;
}
