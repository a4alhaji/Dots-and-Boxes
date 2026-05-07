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
#define STATUS_LEN 80

typedef struct {
    char hLines[ROWS][COLS-1];
    char vLines[ROWS-1][COLS];
    char boxes[ROWS-1][COLS-1];
    char status[STATUS_LEN];
} GameState;

typedef struct {
    int r1;
    int c1;
    int r2;
    int c2;
} Move;

static int sendAll(int sock, const void *buf, size_t len) {
    size_t total = 0;
    const char *p = (const char *)buf;

    while (total < len) {
        ssize_t sent = send(sock, p + total, len - total, 0);
        if (sent <= 0) {
            return -1;
        }
        total += (size_t)sent;
    }

    return 0;
}

static int recvAll(int sock, void *buf, size_t len) {
    size_t total = 0;
    char *p = (char *)buf;

    while (total < len) {
        ssize_t got = recv(sock, p + total, len - total, 0);
        if (got <= 0) {
            return -1;
        }
        total += (size_t)got;
    }

    return 0;
}

int main(int argc, char **argv) {
    int sock;
    struct sockaddr_in serv_addr;

    const char *server_ip = "127.0.0.1";
    if (argc >= 2) {
        server_ip = argv[1];
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket failed");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        printf("Invalid server IP: %s\n", server_ip);
        close(sock);
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return 1;
    }

    char myPlayer;
    if (recvAll(sock, &myPlayer, sizeof(char)) < 0) {
        printf("Failed to receive player ID\n");
        close(sock);
        return 1;
    }

    while (1) {
        GameState state;
        if (recvAll(sock, &state, sizeof(GameState)) < 0) {
            printf("Disconnected from server\n");
            close(sock);
            return 0;
        }

        memcpy(hLines, state.hLines, sizeof(hLines));
        memcpy(vLines, state.vLines, sizeof(vLines));
        memcpy(boxes, state.boxes, sizeof(boxes));

        system("clear");
        printf("You are Player %c\n\n", myPlayer);
        printBoard();
        printf("\n%s\n", state.status);

        if (strcmp(state.status, "GAME OVER") == 0) {
            break;
        }

        if (strstr(state.status, "YOUR TURN") != NULL) {
            Move move;

            printf("Enter r1 c1: ");
            if (scanf("%d %d", &move.r1, &move.c1) != 2) {
                printf("Invalid input\n");
                break;
            }

            printf("Enter r2 c2: ");
            if (scanf("%d %d", &move.r2, &move.c2) != 2) {
                printf("Invalid input\n");
                break;
            }

            if (sendAll(sock, &move, sizeof(Move)) < 0) {
                printf("Failed to send move\n");
                break;
            }

            char response[20];
            if (recvAll(sock, response, sizeof(response)) < 0) {
                printf("Disconnected from server\n");
                break;
            }

            if (strcmp(response, "INVALID") == 0) {
                printf("\nInvalid move!\n");
                sleep(1);
            }
        }
    }

    close(sock);
    return 0;
}
