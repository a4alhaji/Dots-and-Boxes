#include <stdio.h>
#include <stdlib.h>
#include "../include/board.h"

char hLines[ROWS][COLS-1];   // horizontal lines
char vLines[ROWS-1][COLS];   // vertical lines
char boxes[ROWS-1][COLS-1];  // boxes

int dotIndices[ROWS][COLS];

int scoreA = 0;
int scoreB = 0;

void initBoard() {
    int dotCounter = 1;

    // Initialize horizontal lines
    for(int i = 0; i < ROWS; i++)
        for(int j = 0; j < COLS-1; j++)
            hLines[i][j] = ' ';

    // Initialize vertical lines
    for(int i = 0; i < ROWS-1; i++)
        for(int j = 0; j < COLS; j++)
            vLines[i][j] = ' ';

    // Initialize boxes
    for(int i = 0; i < ROWS-1; i++)
        for(int j = 0; j < COLS-1; j++)
            boxes[i][j] = ' ';

    // Assign indices to dots
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            dotIndices[i][j] = dotCounter++;
        }
    }
}

void printBoard() {

    // Print top column indices
    printf("  "); // space for row indices
    for(int j = 0; j < COLS; j++){
        printf("%d ", j);
    }
    printf("\n");

    for(int i = 0; i < ROWS; i++){

        // Print row index
        printf("%d ", i);

        // Print dots + horizontal lines
        for(int j = 0; j < COLS-1; j++){
            printf(".");
            printf("%c", hLines[i][j]); // line or space
        }
        printf(".\n");

        // Print vertical lines + boxes
        if(i < ROWS-1){
            printf("  "); // align with row index spacing
            for(int j = 0; j < COLS-1; j++){
                printf("%c", vLines[i][j]);
                printf("%c", boxes[i][j]);
            }
            printf("%c\n", vLines[i][COLS-1]);
        }
    }
}

int checkBox(int r, int c, char player){
    if(hLines[r][c] != ' ' &&
       hLines[r+1][c] != ' ' &&
       vLines[r][c] != ' ' &&
       vLines[r][c+1] != ' ' &&
       boxes[r][c] == ' ') {

        boxes[r][c] = player;
        if(player == 'A') scoreA++;
        else scoreB++;
        return 1;
    }
    return 0;
}

int drawLine(int r1, int c1, int r2, int c2, char player){
    int scored = 0;

    // horizontal
    if(r1 == r2 && abs(c1 - c2) == 1){
        int c = (c1 < c2) ? c1 : c2;
        if(hLines[r1][c] != ' ') return -1;
        hLines[r1][c] = '-';

        if(r1 > 0) scored += checkBox(r1-1, c, player);
        if(r1 < ROWS-1) scored += checkBox(r1, c, player);
    }

    // vertical
    else if(c1 == c2 && abs(r1 - r2) == 1){
        int r = (r1 < r2) ? r1 : r2;
        if(vLines[r][c1] != ' ') return -1;
        vLines[r][c1] = '|';

        if(c1 > 0) scored += checkBox(r, c1-1, player);
        if(c1 < COLS-1) scored += checkBox(r, c1, player);
    }

    else return -1;

    return scored;
}

int isGameOver(){
    return (scoreA + scoreB) == (ROWS-1)*(COLS-1);
}

int getScoreA(){ return scoreA; }
int getScoreB(){ return scoreB; }