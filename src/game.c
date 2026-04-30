#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "board.h"

int isValidLine(int i, int j){
    // must be empty
    if(grid[i][j] != ' ')
        return 0;

    // horizontal line
    if(i % 2 == 0 && j % 2 == 1)
        return 1;

    // vertical line
    if(i % 2 == 1 && j % 2 == 0)
        return 1;

    return 0;
}

int completesBox(int i, int j){
    char temp = grid[i][j];
    grid[i][j] = (i % 2 == 0) ? '-' : '|';

    int result = 0;

    for(int r = 1; r < ROWS; r += 2){
        for(int c = 1; c < COLS; c += 2){
            if(grid[r][c] == ' '){
                if(grid[r-1][c] == '-' &&
                   grid[r+1][c] == '-' &&
                   grid[r][c-1] == '|' &&
                   grid[r][c+1] == '|'){
                    result = 1;
                }
            }
        }
    }

    grid[i][j] = temp;
    return result;
}

int createsThirdSide(int i, int j){
    char temp = grid[i][j];
    grid[i][j] = (i % 2 == 0) ? '-' : '|';

    int danger = 0;

    for(int r = 1; r < ROWS; r += 2){
        for(int c = 1; c < COLS; c += 2){
            if(grid[r][c] == ' '){
                int sides = 0;

                if(grid[r-1][c] == '-') sides++;
                if(grid[r+1][c] == '-') sides++;
                if(grid[r][c-1] == '|') sides++;
                if(grid[r][c+1] == '|') sides++;

                if(sides == 3){
                    danger = 1;
                }
            }
        }
    }

    grid[i][j] = temp;
    return danger;
}

void hardBotMove(){
    int r1, c1, r2, c2;

    // 1. complete box
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            if(isValidLine(i,j) && completesBox(i,j)){

                // determine endpoints
                if(i % 2 == 0){ // horizontal
                    r1 = i; c1 = j-1;
                    r2 = i; c2 = j+1;
                } else { // vertical
                    r1 = i-1; c1 = j;
                    r2 = i+1; c2 = j;
                }

                drawLine(r1,c1,r2,c2,'B');
                return;
            }
        }
    }

    // 2. safe move
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            if(isValidLine(i,j) && !createsThirdSide(i,j)){

                if(i % 2 == 0){
                    r1 = i; c1 = j-1;
                    r2 = i; c2 = j+1;
                } else {
                    r1 = i-1; c1 = j;
                    r2 = i+1; c2 = j;
                }

                drawLine(r1,c1,r2,c2,'B');
                return;
            }
        }
    }

    // 3. random move
    while(1){
        int i = rand() % ROWS;
        int j = rand() % COLS;

        if(isValidLine(i,j)){
            if(i % 2 == 0){
                r1 = i; c1 = j-1;
                r2 = i; c2 = j+1;
            } else {
                r1 = i-1; c1 = j;
                r2 = i+1; c2 = j;
            }

            drawLine(r1,c1,r2,c2,'B');
            return;
        }
    }
}

void startGame(){
    initBoard();
    int mode;

    printf("Choose mode:\n");
    printf("1. Player vs Player\n");
    printf("2. Player vs Hard Bot\n");
    printf("Enter choice: ");
    scanf("%d", &mode);

    int r1, c1, r2, c2;
    char current = 'A';

    while(!isGameOver()){
        printBoard();

        if(current == 'A'){
            printf("Player A turn\n");

            printf("Enter r1 c1: ");
            scanf("%d %d", &r1, &c1);

            printf("Enter r2 c2: ");
            scanf("%d %d", &r2, &c2);

            int res = drawLine(r1,c1,r2,c2,'A');

            if(res == -1){
                printf("Invalid move!\n");
                continue;
            }

            if(res == 0)
                current = 'B';
        }
        else{
            if(mode == 1){
                printf("Player B turn\n");

                printf("Enter r1 c1: ");
                scanf("%d %d", &r1, &c1);

                printf("Enter r2 c2: ");
                scanf("%d %d", &r2, &c2);

                int res = drawLine(r1,c1,r2,c2,'B');

                if(res == -1){
                    printf("Invalid move!\n");
                    continue;
                }

                if(res == 0)
                    current = 'A';
            }
            else{
                printf("Bot (Player B) turn...\\n");
                sleep(1); 
                hardBotMove();
                current = 'A';
            }
        }
    }

    printBoard();
    printf("Game Over!\n");
}