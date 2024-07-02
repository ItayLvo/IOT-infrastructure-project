#include <stdio.h>	/* printf */

#include "uid.h"


int main()
{
	ilrd_uid_t uid1 = UIDCreate();
	ilrd_uid_t uid2 = UIDCreate();
	
	printf("%ld\n", uid1.timestamp);
	printf("%ld\n", uid1.counter);
	printf("%d\n\n", uid1.pid);
	
	printf("%ld\n", uid2.timestamp);
	printf("%ld\n", uid2.counter);
	printf("%d\n\n", uid2.pid);
	
	printf("testing IsEqual with GetBad (expected result is 0): %d\n", UIDIsEqual((uid1), UIDGetBad()));
	printf("testing IsEqual with two UID_t (expected result is 0) %d\n:", UIDIsEqual(uid1, uid2));
	
	return 0;
}
