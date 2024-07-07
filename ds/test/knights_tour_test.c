#include <stdio.h>	/* printf */
#include <time.h>	/* time() */
#include <stdlib.h>	/* rand, srand, qsort */

#include "knights_tour.h"
#include "bitarray.h"

#define FIVE_MINUTES 300

int main()
{
	position_t path[PATH_LENGTH] = {0};
	position_t starting_position = 36;	/* 0010 0100 */
/*	char new_string[100];*/
	
	size_t i = 0;
	
	printf("status = %d\n", RunKnightsTour(path, starting_position, 0, FIVE_MINUTES));
	
	return 0;
}
