#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

typedef struct {
    int r1, c1, r2, c2;
} Move;

int main() {
    int sock;
    struct sockaddr_in server;
    Move move;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    connect(sock, (struct sockaddr*)&server, sizeof(server));

    printf("Connected to server\n");

    while (1) {
        printf("Enter move (r1 c1 r2 c2): ");
        scanf("%d %d %d %d", &move.r1, &move.c1, &move.r2, &move.c2);

        send(sock, &move, sizeof(move), 0);
    }

    close(sock);
    return 0;
}