#include <time.h>		/* time_t */
#include <stddef.h>		/* size_t */
#include <sys/types.h>	/* pid_t */
#include <assert.h>		/* assert */
#include <unistd.h>		/* getpid */
#include <pthread.h>	/* mutex */
#include "uid.h"


/* managing struct (from .h file): */
/*
struct ilrd_uid {
    time_t timestamp;
    size_t counter; 
    pid_t pid;
};
*/


/* static mutex ensures thread-safe handling of UID static counter */
static pthread_mutex_t uid_mutex = PTHREAD_MUTEX_INITIALIZER;


ilrd_uid_t UIDCreate(void)
{
	ilrd_uid_t uid;
	static size_t count = 0;
	
	time(&uid.timestamp);
	if ((time_t)-1 == uid.timestamp)
	{
		return UIDGetBad();
	}
	
	uid.pid = getpid();
	
	pthread_mutex_lock(&uid_mutex);
	uid.counter = count;
	++count;
	pthread_mutex_unlock(&uid_mutex);

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
