#ifndef BOARD_H
#define BOARD_H

// Board dimensions
#define ROWS 5
#define COLS 6

// Board state arrays
extern char hLines[ROWS][COLS-1];
extern char vLines[ROWS-1][COLS];
extern char boxes[ROWS-1][COLS-1];

// Board functions
void initBoard();
void printBoard();

int drawLine(int r1, int c1, int r2, int c2, char player);
int checkBox(int r, int c, char player);

int isGameOver();

#endif