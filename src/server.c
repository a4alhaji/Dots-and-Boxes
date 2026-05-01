/* =========================
   server.c
   ========================= */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "board.h"

#define PORT 8080

typedef struct {

    char hLines[ROWS][COLS-1];
    char vLines[ROWS-1][COLS];
    char boxes[ROWS-1][COLS-1];

    char status[20];

} GameState;

typedef struct {
    int r1;
    int c1;
    int r2;
    int c2;
} Move;

int client1, client2;

char currentPlayer = 'A';

pthread_mutex_t lock;


    void sendState(int client, char* status){

        GameState state;
        memcpy(state.hLines, hLines, sizeof(hLines));
        memcpy(state.vLines, vLines, sizeof(vLines));
        memcpy(state.boxes, boxes, sizeof(boxes));
        strcpy(state.status, status);
        send(client, &state, sizeof(GameState), 0);
    }


void broadcastBoard(){

    if(currentPlayer == 'A'){

    sendState(client1, "YOUR TURN");
    sendState(client2, "WAIT");
}
else{

    sendState(client1, "WAIT");
    sendState(client2, "YOUR TURN");
}
}

int main(){

    int server_fd;

    struct sockaddr_in address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd < 0){
        perror("Socket failed");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));

    listen(server_fd, 2);

    printf("Waiting for players...\n");

    client1 = accept(server_fd, NULL, NULL);
    printf("Player A connected\n");

    client2 = accept(server_fd, NULL, NULL);
    printf("Player B connected\n");

    char p1 = 'A';
    char p2 = 'B';

    send(client1, &p1, sizeof(char), 0);
    send(client2, &p2, sizeof(char), 0);

    pthread_mutex_init(&lock, NULL);

    initBoard();

    broadcastBoard();

    while(!isGameOver()){

        int currentSocket;

        if(currentPlayer == 'A')
            currentSocket = client1;
        else
            currentSocket = client2;


        Move move;

        recv(currentSocket, &move, sizeof(Move), 0);

        pthread_mutex_lock(&lock);

        int result = drawLine(
            move.r1,
            move.c1,
            move.r2,
            move.c2,
            currentPlayer
        );

        pthread_mutex_unlock(&lock);

        if(result == -1){

            send(currentSocket, "INVALID", 8, 0);

            continue;
        }

        send(currentSocket, "VALID", 6, 0);

        broadcastBoard();

        if(result == 0){

            if(currentPlayer == 'A')
                currentPlayer = 'B';
            else
                currentPlayer = 'A';
        }
    }

    printf("Game Over!\n");

    close(client1);
    close(client2);
    close(server_fd);

    pthread_mutex_destroy(&lock);

    return 0;
}