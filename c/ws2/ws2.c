#include <stdio.h>	/* printf */
#include "ws2.h"	
#include <string.h>

int main()
{
	SevenBoom(1, 50);
	return 0;
}


void SevenBoom(int from, int to)
{
	int i;
	char str[3];
	for (i = from; i < to; i++)
	{
		sprintf(str, "%d", i);
		/* printf("--%s--\n", str); */
		if (strchr(str, 7 + '0') || i % 7 == 0) 	/* the number contains the digit 7 or divisible by 7 */
		{
			printf("BOOM\n");
		}
		else
		{
			printf("%d\n", i);
		}
	}
}
