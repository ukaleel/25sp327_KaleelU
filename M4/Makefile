CC=gcc
CFLAGS=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra
LIBS=-lcmocka

all: input_main test_input

input_main: input_main.o
	@echo "\nLink step to create input_main:"
	gcc $(CFLAGS) -o input_main input_main.o

test_input: input.o test_input.o
	@echo "\nLink step to create test_input:"
	gcc $(CFLAGS) -o test_input test_input.o input.o $(LIBS)

input_main.o: input.c
	gcc $(CFLAGS) -c input.c -o input_main.o

input.o: input.c
	gcc $(CFLAGS) -DUNIT_TEST -c input.c -o input.o

test_input.o: test_input.c
	gcc $(CFLAGS) -DUNIT_TEST -c test_input.c

test: test_input
	@echo
	./test_input

clean:
	@echo
	rm -fv input_main test_input *.o
	@echo

.PHONY: all clean
