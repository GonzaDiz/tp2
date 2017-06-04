EXEC = tweet
CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror 

all: 
	$(CC) $(CFLAGS) -o $(EXEC) counting_filters.c hash.c heap.c pila.c procesar_tweets.c strutil.c lib.c

clean: 
		rm $(EXEC) *.o

run: all

	valgrind --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 cat tweets.txt | ./tweet 300000 20
