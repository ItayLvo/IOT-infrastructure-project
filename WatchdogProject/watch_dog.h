#ifndef __WATCH_DOG_H__
#define __WATCH_DOG_H__

int MMI(size_t interval_in_seconds, size_t repititions, char **argv);	/* first argument of argv is the user application path */

void DNR(void);

#endif /*__WATCH_DOG_H__*/
