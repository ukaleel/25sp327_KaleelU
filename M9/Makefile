CC = gcc
LINKER = gcc
CFLAGS = -Wall -Wextra -O0
LDFLAGS = -g
LIBS = -lpthread

all: request_server test_queue

tasks.o: tasks.c tasks.h
	$(CC) $(CFLAGS) -c tasks.c -o tasks.o

request_server: request_server.o thread_pool.o worker_thread.o queue.o tasks.o
	$(LINKER) $(LDFLAGS) request_server.o thread_pool.o worker_thread.o queue.o tasks.o -o request_server $(LIBS)

queue.o: queue.c queue_internal.h queue.h common.h
	$(CC) $(CFLAGS) -c queue.c -o queue.o

test_queue.o: test_queue.c queue_internal.h queue.h
	$(CC) $(CFLAGS) -c test_queue.c -o test_queue.o

test_queue: test_queue.o queue.o
	$(LINKER) $(LDFLAGS) test_queue.o queue.o -o test_queue -lcmocka -Wl,--wrap=pthread_mutex_lock,--wrap=pthread_mutex_unlock

request_server.o: request_server.c common.h queue.h tasks.h thread_pool.h worker_thread.h
	$(CC) $(CFLAGS) -c request_server.c -o request_server.o

thread_pool.o: thread_pool.c thread_pool.h queue.h worker_thread.h
	$(CC) $(CFLAGS) -c thread_pool.c -o thread_pool.o

worker_thread.o: worker_thread.c worker_thread.h queue.h common.h
	$(CC) $(CFLAGS) -c worker_thread.c -o worker_thread.o

clean:
	rm -f *.o request_server test_queue