/* =========================
   client.c
   ========================= */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

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

int main(){

    int sock;

    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    char myPlayer;

    recv(sock, &myPlayer, sizeof(char), 0);

    printf("You are Player %c\n", myPlayer);

  while(1){

    GameState state;

    int total = 0;
    int bytes;

    while(total < sizeof(GameState)){

        bytes = recv(
            sock,
            ((char*)&state) + total,
            sizeof(GameState) - total,
            0
        );

        if(bytes <= 0){
            printf("Disconnected from server\n");
            close(sock);
            return 0;
        }

        total += bytes;
    }

    memcpy(hLines, state.hLines, sizeof(hLines));
    memcpy(vLines, state.vLines, sizeof(vLines));
    memcpy(boxes, state.boxes, sizeof(boxes));

    system("clear");

    printf("You are Player %c\n\n", myPlayer);

    printBoard();

    printf("\n%s\n", state.status);

    // =====================
    // YOUR TURN
    // =====================
    if(strcmp(state.status, "YOUR TURN") == 0){

        Move move;

        printf("Enter r1 c1: ");
        scanf("%d %d", &move.r1, &move.c1);

        printf("Enter r2 c2: ");
        scanf("%d %d", &move.r2, &move.c2);

        send(sock, &move, sizeof(Move), 0);

        char response[20];

        recv(sock, response, sizeof(response), 0);

        if(strcmp(response, "INVALID") == 0){

            printf("\nInvalid move!\n");

            sleep(1);
        }
    }
}

    close(sock);

    return 0;
}