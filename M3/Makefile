CC = gcc
CFLAGS = -Wall -Wextra

all: compare_static compare_dynamic


compare_static: compare.o
	$(CC) -static -o compare_static compare.o


compare_dynamic: compare.o
		$(CC) -o compare_dynamic compare.o


compare.o: compare.c
	$(CC) $(CFLAGS) -c compare.c


.PHONY: clean
clean:
	rm -fv compare_static compare_dynamic *.o


.PHONY: compare
compare: 
	@echo "compare_static: "
	ls -l | grep compare_s
	@echo "compare_dynamic: "
	ls -l | grep compare_d
	./compare_static
	./compare_dynamic
