CC = gcc
CFLAGS = -Wall -Wextra -g

all: setuid_demo

setuid_demo: setuid_demo.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f setuid_demo

test: all
	@echo "Setting up setuid-root permissions..."
	sudo chown root setuid_demo
	sudo chmod u+s setuid_demo
	ls -l setuid_demo
	@echo "Running the program as a non-root user..."
	./setuid_demo
