CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -ggdb
LIBPATH = .
OBJS = watch_dog.o scheduler.o dllist.o sorted_list.o priority_queue.o uid.o task.o
HEADER_DIR = /home/itay/git/ds/include/
SRC_DIR = /home/itay/git/ds/src/
TEST_DIR = /home/itay/git/ds/test/
PROJ_DIR = /home/itay/git/projects/

#make debug and release versions (as a client)
#all : debug release

#make shared lib only:
all : libwatchdog.so watchdog_exec.out

debug : CFLAGS =
debug : CFLAGS += -ansi -pedantic-errors -Wall -Wextra -ggdb
debug : $(FN)_test.o libdatastructurelib.so
	$(CC) $(CFLAGS) -o debug.out $(FN)_test.o -L $(LIBPATH) -l watchdog

release : CFLAGS =
release : CFLAGS += -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -o3
release : $(FN)_test.o libdatastructurelib.so
	$(CC) $(CFLAGS) -o release.out $(FN)_test.o -L $(LIBPATH) -l watchdog

test :
#####
# Test files compiled to object files:

$(FN)_test.o : $(TEST_DIR)$(FN)_test.c $(HEADER_DIR)$(FN).h
	$(CC) $(CFLAGS) -c -o $@ $(TEST_DIR)$(FN)_test.c -I $(HEADER_DIR)
	
	
	
#####
# Shared object compiled from object files:

libwatchdog.so : $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $(OBJS) -I $(HEADER_DIR) -I $(PROJ_DIR) -lpthread



###
# seperate executable for watchdog process:
watchdog_exec.o : $(PROJ_DIR)wd_process.c watch_dog.o scheduler.o priority_queue.o task.o uid.o $(HEADER_DIR)scheduler.h $(HEADER_DIR)priority_queue.h $(HEADER_DIR)task.h $(HEADER_DIR)uid.h $(PROJ_DIR)watch_dog.h
	$(CC) $(CFLAGS) -c -o $@ $(PROJ_DIR)wd_process.c -I $(HEADER_DIR) -I $(PROJ_DIR)
	
watchdog_exec.out : watchdog_exec.o $(PROJ_DIR)watch_dog.h
	$(CC) $(CFLAGS) $(PROJ_DIR)watchdog_exec.o $(OBJS) -o $@ -lpthread
	


#####
# Src files compiled to object files:
watch_dog.o : $(PROJ_DIR)watch_dog.c scheduler.o priority_queue.o task.o uid.o $(HEADER_DIR)scheduler.h $(HEADER_DIR)priority_queue.h $(HEADER_DIR)task.h $(HEADER_DIR)uid.h $(PROJ_DIR)watch_dog.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $(PROJ_DIR)watch_dog.c -I $(HEADER_DIR) -I $(PROJ_DIR) -lpthread


scheduler.o : $(SRC_DIR)scheduler.c priority_queue.o task.o uid.o $(HEADER_DIR)scheduler.h $(HEADER_DIR)priority_queue.h $(HEADER_DIR)task.h $(HEADER_DIR)uid.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $(SRC_DIR)scheduler.c -I $(HEADER_DIR)


task.o : $(SRC_DIR)task.c uid.o $(HEADER_DIR)task.h $(HEADER_DIR)uid.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $(SRC_DIR)task.c -I $(HEADER_DIR)


uid.o : $(SRC_DIR)uid.c $(HEADER_DIR)uid.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $(SRC_DIR)uid.c -I $(HEADER_DIR)


priority_queue.o : $(SRC_DIR)priority_queue.c sorted_list.o $(HEADER_DIR)priority_queue.h $(HEADER_DIR)sorted_list.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $(SRC_DIR)priority_queue.c -I $(HEADER_DIR)
	

sorted_list.o : $(SRC_DIR)sorted_list.c dllist.o $(HEADER_DIR)sorted_list.h $(HEADER_DIR)dllist.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $(SRC_DIR)sorted_list.c -I $(HEADER_DIR)


dllist.o : $(SRC_DIR)dllist.c $(HEADER_DIR)dllist.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $(SRC_DIR)dllist.c -I $(HEADER_DIR)


queue.o : $(SRC_DIR)queue.c $(HEADER_DIR)queue.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $(SRC_DIR)queue.c -I $(HEADER_DIR)


sllist.o : $(SRC_DIR)sllist.c $(HEADER_DIR)sllist.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $(SRC_DIR)sllist.c -I $(HEADER_DIR)




.PHONY : clean
clean :
	rm ./$(OBJS) $(PROJ_DIR)watchdog_exec.o
#	rm ./*.o
