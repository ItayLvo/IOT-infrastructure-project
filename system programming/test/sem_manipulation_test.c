#include <stdio.h>	/* printf */
#include "sem_manipulation.h"


int main(int argc, char *argv[])
{
	int return_status = SUCCESS;
	(void)argc;
	
	return_status = SemRun(argv[1]);
	
	if (return_status != SUCCESS)
	{
		printf("Exited with status: %d\n", return_status);
	}
	
	return return_status;
}

