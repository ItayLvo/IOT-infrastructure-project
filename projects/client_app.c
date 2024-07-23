#include <stddef.h>	/* size_t */
#include <stdio.h>	/* printf */

#include "watch_dog.h"

#define INTERVAL 3
#define EXEC_PATH "/home/itay/git/projects/client_app"



int main()
{
	MMI(INTERVAL, EXEC_PATH, NULL);
	
	
	DNR();
	
	return 0;
}
