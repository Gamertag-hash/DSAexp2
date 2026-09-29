CC ?= gcc
CFLAGS ?= -Wall -Wextra -O2

all: main

main: main.c kway_merge.c pairwise_merge.c merge.h
	$(CC) $(CFLAGS) -o main main.c kway_merge.c pairwise_merge.c -lm

clean:
	rm -f main
