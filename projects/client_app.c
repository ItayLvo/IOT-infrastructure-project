#include <stddef.h>	/* size_t */
#include <stdio.h>	/* printf */

#include "watch_dog.h"

#define INTERVAL 3
#define REPETITIONS 3
#define EXEC_PATH "/home/itay/git/projects/user_exec.out"



int main()
{
	int status = 0;
	size_t i = 0;
	char *file_name = EXEC_PATH;
	char *arg1 = "test1";
	char *arg2 = "test2";
	char *argv[4] = {0};
	
	argv[0] = file_name;
	argv[1] = arg1;
	argv[2] = arg2;
	argv[3] = NULL;


	status = MMI(INTERVAL, REPETITIONS, argv);
	printf("client process, main, after returned from MMI\n");
	while(i<25)
	{
/*		printf("client process, main thread. seconds passed = %lu\n", i);*/
		++i;
		printf("%lu\n", i);
		sleep(1);
	}
	
	DNR();
	
	return status;
}
