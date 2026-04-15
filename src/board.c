#include <stdio.h>
#include "board.h"

char grid[ROWS][COLS];

void initBoard(){
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            if(i % 2 == 0 && j % 2 == 0)
                grid[i][j] = '.';
            else
                grid[i][j] = ' ';
        }
    }
}

void printBoard(){
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            printf("%c", grid[i][j]);
        }
        printf("\n");
    }
}

int drawLine(int r1, int c1, int r2, int c2, char player){
    if(r1 == r2){ // horizontal
        if(grid[r1][(c1+c2)/2] != ' ')
            return -1;
        grid[r1][(c1+c2)/2] = '-';
    }
    else if(c1 == c2){ // vertical
        if(grid[(r1+r2)/2][c1] != ' ')
            return -1;
        grid[(r1+r2)/2][c1] = '|';
    }
    else return -1;

    return checkBoxes(player);
}

int checkBoxes(char player){
    int scored = 0;

    for(int i = 1; i < ROWS; i += 2){
        for(int j = 1; j < COLS; j += 2){
            if(grid[i][j] == ' '){
                if(grid[i-1][j] == '-' &&
                   grid[i+1][j] == '-' &&
                   grid[i][j-1] == '|' &&
                   grid[i][j+1] == '|'){
                    
                    grid[i][j] = player;
                    scored = 1;
                }
            }
        }
    }
    return scored;
}

int isGameOver(){
    for(int i = 1; i < ROWS; i += 2){
        for(int j = 1; j < COLS; j += 2){
            if(grid[i][j] == ' ')
                return 0;
        }
    }
    return 1;
}