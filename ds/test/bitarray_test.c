#include <stdio.h>	/* printf */

#include "bitarray.h"


int main()
{
	bitarray bitarr = {0};
	bitarray *bitarr_ptr = &bitarr;
	bitarr.data = 0;
	set_all(bitarr_ptr);
	to_string(bitarr_ptr); /*****/
	
	return 0;
}
