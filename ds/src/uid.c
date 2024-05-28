#include <time.h>	/* time_t */
#include <stddef.h>	/* size_t */
#include <sys/types.h>	/* pid_t */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */
#include <unistd.h>	/* getpid */

#include "uid.h"

/*
struct ilrd_uid {
    time_t timestamp;
    size_t counter; 
    pid_t pid;
};
*/

ilrd_uid_t UIDCreate(void)
{
	ilrd_uid_t uid;
	char time_str[80];
	static size_t count = 0;
	struct tm *timeinfo; /* used to convert time to human-readable data */
	
	time(&uid.timestamp);
	if ((time_t)-1 == uid.timestamp)
	{
		return UIDGetBad();
	}
	timeinfo = localtime(&uid.timestamp);

	if (0 == strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo))
	{
		return UIDGetBad();
	}
	
	uid.pid = getpid();
	
	uid.counter = count;
	++count;

	return uid;
}


int UIDIsEqual(ilrd_uid_t uid1, ilrd_uid_t uid2)
{
	return (uid1.counter == uid2.counter &&
	uid1.pid == uid2.pid &&
	uid1.timestamp == uid2.timestamp);
}

ilrd_uid_t UIDGetBad(void)
{
	ilrd_uid_t uid;
	uid.counter = 0;
	uid.pid = 0;
	uid.timestamp = 0;
	
	return uid;
}
