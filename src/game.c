#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "board.h"
#include "game.h"

// =====================
// Bot Move
// =====================
void hardBotMove(){

    while(1){

        int r1 = rand() % ROWS;
        int c1 = rand() % COLS;

        int direction = rand() % 2;

        int r2 = r1;
        int c2 = c1;

        // horizontal
        if(direction == 0){
            c2 = c1 + 1;
        }

        // vertical
        else{
            r2 = r1 + 1;
        }

        int result = drawLine(r1, c1, r2, c2, 'B');

        if(result != -1)
            return;
    }
}

// =====================
// Main Game Loop
// =====================
void startGame(){

    srand(time(NULL));

    initBoard();

    int mode;

    printf("Choose mode:\n");
    printf("1. Player vs Player\n");
    printf("2. Player vs Bot\n");
    printf("Enter choice: ");

    scanf("%d", &mode);

    int r1, c1, r2, c2;

    char current = 'A';

    while(!isGameOver()){

        printBoard();

        // =====================
        // PLAYER A
        // =====================
        if(current == 'A'){

            printf("Player A turn\n");

            printf("Enter r1 c1: ");
            scanf("%d %d", &r1, &c1);

            printf("Enter r2 c2: ");
            scanf("%d %d", &r2, &c2);

            int result = drawLine(r1, c1, r2, c2, 'A');

            if(result == -1){
                printf("Invalid move!\n");
                continue;
            }

            if(result == 0)
                current = 'B';
        }

        // =====================
        // PLAYER B / BOT
        // =====================
        else{

            // PvP
            if(mode == 1){

                printf("Player B turn\n");

                printf("Enter r1 c1: ");
                scanf("%d %d", &r1, &c1);

                printf("Enter r2 c2: ");
                scanf("%d %d", &r2, &c2);

                int result = drawLine(r1, c1, r2, c2, 'B');

                if(result == -1){
                    printf("Invalid move!\n");
                    continue;
                }

                if(result == 0)
                    current = 'A';
            }

            // Bot
            else{

                printf("Bot turn...\n");

                sleep(1);

                hardBotMove();

                current = 'A';
            }
        }
    }

    printBoard();

    printf("Game Over!\n");
}