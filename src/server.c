#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "board.h"

#define PORT 8080

typedef struct {
    int r1, c1, r2, c2;
} Move;

int playerSockets[2];
char currentPlayer = 'A';

pthread_mutex_t lock;

void *handlePlayer(void *arg) {
    int sock = *(int*)arg;
    char player;

    if (sock == playerSockets[0]) player = 'A';
    else player = 'B';

    Move move;

    while (!isGameOver()) {

        if (player != currentPlayer) continue;

        // receive move
        recv(sock, &move, sizeof(move), 0);

        pthread_mutex_lock(&lock);

        int result = drawLine(move.r1, move.c1, move.r2, move.c2, player);

        if (result != -1) {
            if (result == 0)
                currentPlayer = (currentPlayer == 'A') ? 'B' : 'A';

            printBoard();

            // send update to both players
            send(playerSockets[0], &move, sizeof(move), 0);
            send(playerSockets[1], &move, sizeof(move), 0);
        }

        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main() {
    int server_fd, addrlen;
    struct sockaddr_in address;

    initBoard();
    pthread_mutex_init(&lock, NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 2);

    printf("Waiting for players...\n");

    addrlen = sizeof(address);

    playerSockets[0] = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    printf("Player A connected\n");

    playerSockets[1] = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    printf("Player B connected\n");

    pthread_t t1, t2;

    pthread_create(&t1, NULL, handlePlayer, &playerSockets[0]);
    pthread_create(&t2, NULL, handlePlayer, &playerSockets[1]);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    close(server_fd);
    return 0;
}