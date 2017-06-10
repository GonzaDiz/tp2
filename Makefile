EXEC = procesar_tweets
EXEC1 = procesar_usuarios
CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror 

all: tweets usuarios

tweets:
	$(CC) $(CFLAGS) -o $(EXEC) lib.c cmsketch.c strutil.c pila.c heap.c hash.c procesar_tweets.c

usuarios: 
	$(CC) $(CFLAGS) -o $(EXEC1) hash.c lista.c procesar_usuarios.c

clean: 
		rm $(EXEC) $(EXEC1) *.o

run: all

	valgrind --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 cat tweets_head.txt | ./procesar_tweets 20 5

	valgrind --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 ./procesar_usuarios tweets_head.txt
