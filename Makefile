# =========================
# Makefile
# =========================

CC = gcc
CFLAGS = -Wall -Werror -g

all: server client

server: server.o board.o
	$(CC) $(CFLAGS) -o server server.o board.o -lpthread

client: client.o board.o
	$(CC) $(CFLAGS) -o client client.o board.o

server.o: src/server.c src/board.h
	$(CC) $(CFLAGS) -c src/server.c

client.o: src/client.c src/board.h
	$(CC) $(CFLAGS) -c src/client.c

board.o: src/board.c src/board.h
	$(CC) $(CFLAGS) -c src/board.c

clean:
	rm -f *.o server client