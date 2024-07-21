#include <stdio.h>	/* printf */

#include "producers_consumers.h"


int main()
{
	int return_status = SUCCESS;
	
/*	RunSpinLock();*/
/*	RunMutex();*/
/*	RunSemaphore();*/
/*	RunTwoSemaphores();*/
/*	RunThreadSafeFSQ();*/
	RunConditionalVariable();
		
	if (return_status != SUCCESS)
	{
		printf("Exited with status: %d\n", return_status);
	}
	
	return return_status;
}

