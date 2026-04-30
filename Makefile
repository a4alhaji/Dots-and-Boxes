CC = gcc
CFLAGS = -Wall -g



all: game

run: game
	./game

	
game: main.o game.o board.o
	$(CC) $(CFLAGS) -o game main.o game.o board.o 

main.o: src/main.c src/game.h
	$(CC) $(CFLAGS) -c src/main.c

game.o: src/game.c src/game.h src/board.h
	$(CC) $(CFLAGS) -c src/game.c

board.o: src/board.c src/board.h
	$(CC) $(CFLAGS) -c src/board.c

server: src/server.c src/board.c
	$(CC) $(CFLAGS) -o server src/server.c src/board.c -lpthread

client: src/client.c
	$(CC) $(CFLAGS) -o client src/client.c

clean:
	rm -f *.o game server client