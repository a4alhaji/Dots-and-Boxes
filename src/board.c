#include <stdio.h>
#include <stdlib.h>
#include "board.h"

int checkBox(int r, int c, char player);

// =====================
// Global Board State
// =====================
char hLines[ROWS][COLS-1];
char vLines[ROWS-1][COLS];
char boxes[ROWS-1][COLS-1];

// =====================
// Initialize Board
// =====================
void initBoard() {
    for(int i = 0; i < ROWS; i++)
        for(int j = 0; j < COLS-1; j++)
            hLines[i][j] = ' ';

    for(int i = 0; i < ROWS-1; i++)
        for(int j = 0; j < COLS; j++)
            vLines[i][j] = ' ';

    for(int i = 0; i < ROWS-1; i++)
        for(int j = 0; j < COLS-1; j++)
            boxes[i][j] = ' ';
}

// =====================
// Print Board
// =====================
void printBoard() {

    // Column indices
    printf("   ");
    for(int j = 0; j < COLS; j++)
        printf("%d ", j);
    printf("\n");

    for(int i = 0; i < ROWS; i++) {

        // Row index
        printf("%d  ", i);

        // Dots + horizontal lines
        for(int j = 0; j < COLS-1; j++) {
            printf(".");
            printf("%c", hLines[i][j]);
        }
        printf(".\n");

        // Vertical lines + boxes
        if(i < ROWS-1) {
            printf("   ");
            for(int j = 0; j < COLS-1; j++) {
                printf("%c", vLines[i][j]);
                printf("%c", boxes[i][j]);
            }
            printf("%c\n", vLines[i][COLS-1]);
        }
    }
}

// =====================
// Draw Line
// =====================
int drawLine(int r1, int c1, int r2, int c2, char player){

    // Bounds check
    if(r1 < 0 || r1 >= ROWS || r2 < 0 || r2 >= ROWS ||
       c1 < 0 || c1 >= COLS || c2 < 0 || c2 >= COLS)
        return -1;

    // Must be adjacent
    if (!((r1 == r2 && abs(c1 - c2) == 1) ||
          (c1 == c2 && abs(r1 - r2) == 1)))
        return -1;

    int scored = 0;

    // Horizontal line
    if(r1 == r2){
        int c = (c1 < c2) ? c1 : c2;

        if(c >= COLS-1 || hLines[r1][c] != ' ')
            return -1;

        hLines[r1][c] = '-';

        if(r1 > 0) scored += checkBox(r1-1, c, player);
        if(r1 < ROWS-1) scored += checkBox(r1, c, player);
    }

    // Vertical line
    else{
        int r = (r1 < r2) ? r1 : r2;

        if(r >= ROWS-1 || vLines[r][c1] != ' ')
            return -1;

        vLines[r][c1] = '|';

        if(c1 > 0) scored += checkBox(r, c1-1, player);
        if(c1 < COLS-1) scored += checkBox(r, c1, player);
    }

    return scored;
}

// =====================
// Check Box Completion
// =====================
int checkBox(int r, int c, char player){
    if(hLines[r][c] != ' ' &&
       hLines[r+1][c] != ' ' &&
       vLines[r][c] != ' ' &&
       vLines[r][c+1] != ' ' &&
       boxes[r][c] == ' ') {

        boxes[r][c] = player;
        return 1;
    }
    return 0;
}

// =====================
// Game Over Check
// =====================
int isGameOver(){
    for(int i = 0; i < ROWS-1; i++){
        for(int j = 0; j < COLS-1; j++){
            if(boxes[i][j] == ' ')
                return 0;
        }
    }
    return 1;
}