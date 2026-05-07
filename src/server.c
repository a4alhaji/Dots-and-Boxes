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

int client1, client2;
char currentPlayer = 'A';
pthread_mutex_t lock;

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

static void sendState(int client, const char *status) {
    GameState state;
    memcpy(state.hLines, hLines, sizeof(hLines));
    memcpy(state.vLines, vLines, sizeof(vLines));
    memcpy(state.boxes, boxes, sizeof(boxes));
    snprintf(state.status, sizeof(state.status), "%s", status);
    sendAll(client, &state, sizeof(GameState));
}

static void broadcastBoard(const char *statusA, const char *statusB) {
    sendState(client1, statusA);
    sendState(client2, statusB);
}

int main() {
    int server_fd;
    struct sockaddr_in address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(1);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(1);
    }

    if (listen(server_fd, 2) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(1);
    }

    printf("Waiting for players...\n");

    client1 = accept(server_fd, NULL, NULL);
    if (client1 < 0) {
        perror("Accept player A failed");
        close(server_fd);
        exit(1);
    }
    printf("Player A connected\n");

    client2 = accept(server_fd, NULL, NULL);
    if (client2 < 0) {
        perror("Accept player B failed");
        close(client1);
        close(server_fd);
        exit(1);
    }
    printf("Player B connected\n");

    char p1 = 'A';
    char p2 = 'B';
    if (sendAll(client1, &p1, sizeof(char)) < 0 ||
        sendAll(client2, &p2, sizeof(char)) < 0) {
        printf("Failed to send player IDs\n");
        close(client1);
        close(client2);
        close(server_fd);
        exit(1);
    }

    pthread_mutex_init(&lock, NULL);

    initBoard();
    broadcastBoard("YOUR TURN", "WAIT: Opponent turn");

    while (!isGameOver()) {
        int currentSocket = (currentPlayer == 'A') ? client1 : client2;
        Move move;

        if (recvAll(currentSocket, &move, sizeof(Move)) < 0) {
            printf("Player %c disconnected\n", currentPlayer);
            break;
        }

        pthread_mutex_lock(&lock);
        int result = drawLine(move.r1, move.c1, move.r2, move.c2, currentPlayer);
        pthread_mutex_unlock(&lock);

        char response[20];

        if (result == -1) {
            snprintf(response, sizeof(response), "INVALID");
            sendAll(currentSocket, response, sizeof(response));

            if (currentPlayer == 'A') {
                broadcastBoard("INVALID MOVE - YOUR TURN", "WAIT: Opponent turn");
            } else {
                broadcastBoard("WAIT: Opponent turn", "INVALID MOVE - YOUR TURN");
            }
            continue;
        }

        snprintf(response, sizeof(response), "VALID");
        sendAll(currentSocket, response, sizeof(response));

        if (result > 0) {
            if (currentPlayer == 'A') {
                broadcastBoard("BOX COMPLETED - YOUR TURN AGAIN", "WAIT: Opponent scored and plays again");
            } else {
                broadcastBoard("WAIT: Opponent scored and plays again", "BOX COMPLETED - YOUR TURN AGAIN");
            }
        } else {
            currentPlayer = (currentPlayer == 'A') ? 'B' : 'A';
            if (currentPlayer == 'A') {
                broadcastBoard("YOUR TURN", "WAIT: Opponent turn");
            } else {
                broadcastBoard("WAIT: Opponent turn", "YOUR TURN");
            }
        }
    }

    broadcastBoard("GAME OVER", "GAME OVER");
    printf("Game Over!\n");

    close(client1);
    close(client2);
    close(server_fd);
    pthread_mutex_destroy(&lock);

    return 0;
}
