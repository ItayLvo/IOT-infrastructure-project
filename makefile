CC = gcc
CFLAGS =
LIBPATH = .
#OBJS = $(wildcard *.o)
OBJS = scheduler.o dllist.o sorted_list.o priority_queue.o uid.o task.o bitarray.o cbuffer.o dvector.o queue.o sllist.o stack.o
HEADER_DIR = /home/itay/git/ds/include/
SRC_DIR = /home/itay/git/ds/src/
TEST_DIR = /home/itay/git/ds/test/

all : debug release

#make shared lib only:
#all : libdatastructurelib.so

debug : CFLAGS =
debug : CFLAGS += -ansi -pedantic-errors -Wall -Wextra -ggdb
debug : $(FN)_test.o libdatastructurelib.so
	$(CC) $(CFLAGS) -o debug.out $(FN)_test.o -L $(LIBPATH) -l datastructurelib

release : CFLAGS =
release : CFLAGS += -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -o3
release : $(FN)_test.o libdatastructurelib.so
	$(CC) $(CFLAGS) -o release.out $(FN)_test.o -L $(LIBPATH) -l datastructurelib

test :



#####
# Test files compiled to object files:

$(FN)_test.o : $(TEST_DIR)$(FN)_test.c $(HEADER_DIR)$(FN).h
	$(CC) $(CFLAGS) -c -o $@ $(TEST_DIR)$(FN)_test.c -I $(HEADER_DIR)
	
	
	
#####
# Shared object compiled from DS object files:

libdatastructurelib.so : $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $(OBJS) -I $(HEADER_DIR)



#####
# DS src files compiled to object files:

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


bitarray.o : $(SRC_DIR)bitarray.c $(HEADER_DIR)bitarray.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $(SRC_DIR)bitarray.c -I $(HEADER_DIR)


cbuffer.o : $(SRC_DIR)cbuffer.c $(HEADER_DIR)cbuffer.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $(SRC_DIR)cbuffer.c -I $(HEADER_DIR)


dvector.o : $(SRC_DIR)dvector.c $(HEADER_DIR)dvector.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $(SRC_DIR)dvector.c -I $(HEADER_DIR)


queue.o : $(SRC_DIR)queue.c $(HEADER_DIR)queue.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $(SRC_DIR)queue.c -I $(HEADER_DIR)


sllist.o : $(SRC_DIR)sllist.c $(HEADER_DIR)sllist.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $(SRC_DIR)sllist.c -I $(HEADER_DIR)


stack.o : $(SRC_DIR)stack.c $(HEADER_DIR)stack.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $(SRC_DIR)stack.c -I $(HEADER_DIR)




.PHONY : clean
clean :
	rm ./$(OBJS)
	rm ./*.o
