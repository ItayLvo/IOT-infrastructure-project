#include <stdio.h>	/* printf */

#include "uid.h"


int main()
{
	ilrd_uid_t uid1 = UIDCreate();
	ilrd_uid_t uid2 = UIDCreate();
	
	printf("%ld\n", uid1.timestamp);
	printf("%ld\n", uid1.counter);
	printf("%d\n", uid1.pid);
	
	printf("%ld\n", uid2.timestamp);
	printf("%ld\n", uid2.counter);
	printf("%d\n", uid2.pid);
	
	return 0;
}
