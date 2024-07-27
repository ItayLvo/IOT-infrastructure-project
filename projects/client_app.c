#include <stddef.h>	/* size_t */
#include <stdio.h>	/* printf */

#include "watch_dog.h"

#define INTERVAL 3
#define REPETITIONS 3
/*#define EXEC_PATH "/home/itay/git/projects/client_app"*/



int main(int argc, char *argv[])
{
	size_t i = 0;
	
	int status = MMI(INTERVAL, REPETITIONS, argv);
	printf("client process, main, after returned from MMI\n");
	while(i < 5)
	{
/*		printf("client process, main thread. seconds passed = %lu\n", i);*/
		++i;
		printf("%lu\n", i);
		sleep(1);
	}
	
	DNR();
	
	(void)argc;
	return status;
}
