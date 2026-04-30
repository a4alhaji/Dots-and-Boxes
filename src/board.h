#ifndef BOARD_H
#define BOARD_H
#define ROWS 5
#define COLS 6

extern char grid[ROWS][COLS];

void initBoard();
void printBoard();
int drawLine(int r1, int c1, int r2, int c2, char player);
int checkBoxes(char player);
int isGameOver();

#endif