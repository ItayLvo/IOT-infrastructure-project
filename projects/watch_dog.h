#ifndef __WATCH_DOG_H__
#define __WATCH_DOG_H__

int MMI(time_t interval_in_seconds, const char *executable_pathname, char **argv);

void DNR(void);

#endif /*__WATCH_DOG_H__*/
