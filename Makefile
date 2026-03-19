CC=gcc
CFLAGS=-Wall -g

all: game

game: main.o board.o game.o
	$(CC) $(CFLAGS) -o game main.o board.o game.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

board.o: board.c
	$(CC) $(CFLAGS) -c board.c

game.o: game.c
	$(CC) $(CFLAGS) -c game.c

clean:
	rm -f *.o game