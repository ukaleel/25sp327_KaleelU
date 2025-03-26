CC=gcc
CFLAGS=-Wall -Wextra
LDFLAGS=-lpthread

ALL=primes1


all: ${ALL}


primes1: primes1.c
	${CC} ${CFLAGS} primes1.c -o primes1 $(LDFLAGS)

clean:
	rm -vf *.o ${ALL}