#ifndef BOARD_H
#define BOARD_H

#define ROWS 9
#define COLS 11

extern char grid[ROWS][COLS];

void initBoard();
void printBoard();
int drawLine(int r1, int c1, int r2, int c2, char player);
int checkBoxes(char player);
int isGameOver();

#endif