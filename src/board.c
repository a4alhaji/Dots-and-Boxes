#include "../include/board.h"

#include <stdio.h>

// printBoard(){}

void initBoard() {
    int boxRows = 4;
    int boxCols = 5;

    int rows = 2*boxRows + 1; // 9
    int cols = 2*boxCols + 1; // 11

    char board[rows][cols];

    // Initialize board with dots and spaces
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(i % 2 == 0 && j % 2 == 0)
                board[i][j] = '.'; // dot
            else
                board[i][j] = ' '; // empty space for lines
        }
    }

    // Print column numbers on top
    printf("   "); // space for row numbers
    for(int j = 0; j < cols; j++) {
        if(j % 2 == 0)
            printf("%d ", j/2);
        else
            printf("  ");
    }
    printf("\n");

    // Print the board with row numbers on the left
    for(int i = 0; i < rows; i++) {
        if(i % 2 == 0)
            printf("%d ", i/2); // row number
        else
            printf("  "); // empty space for in-between rows

        for(int j = 0; j < cols; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }

  
}

// placeLine(){

// }

// checkBox(){

// }