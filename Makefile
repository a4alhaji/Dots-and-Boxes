CC = gcc
CFLAGS = -Wall -Werror -g


all: game
	

game: main.o game.o board.o
	$(CC) $(CFLAGS) -o game main.o game.o board.o 

main.o: src/main.c include/game.h
	$(CC) $(CFLAGS) -c src/main.c

game.o: src/game.c include/game.h include/board.h
	$(CC) $(CFLAGS) -c src/game.c

board.o: src/board.c include/board.h
	$(CC) $(CFLAGS) -c src/board.c

clean:
	rm -f *.o game