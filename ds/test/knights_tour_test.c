#include <stdio.h>	/* printf */

#include "knights_tour.h"
#include "bitarray.h"

#define FIVE_MINUTES 5*60


static unsigned char ParseX(position_t current_pos)
{
	unsigned char x = (current_pos >> 4);
	
	return x;
}


static unsigned char ParseY(position_t current_pos)
{
	unsigned char y = (current_pos << 4);
	
	y >>= 4;
	
	return y;
}



int main()
{
	position_t path[PATH_LENGTH] = {0};
	position_t starting_position1 = 36;	/* 0010 0110 */
	position_t starting_position2 = 0;	/* 0000 0000 */
	position_t starting_position3 = 2;	/* 0000 0010 */
	position_t starting_position4 = 86; /* 0101 0101 */
	position_t starting_position5 = 51; /* 0011 0011 */
	
	size_t i = 0;
	
	printf("\n\nstatus = %d\n", RunKnightsTour(path, starting_position2, 0, FIVE_MINUTES));
	
	for (i = 0; i < PATH_LENGTH; ++i)
	{
		printf("move number: %lu\t\t x = %d | y = %d\n", i, ParseX(path[i]), ParseY(path[i]));
	}
	
	return 0;
}


