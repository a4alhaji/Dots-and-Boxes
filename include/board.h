#ifndef BOARD_H
#define BOARD_H

#define ROWS 5   // dots rows
#define COLS 6   // dots cols

void initBoard();
void printBoard();
int drawLine(int r1, int c1, int r2, int c2, char player);
int isGameOver();
int getScoreA();
int getScoreB();

#endif
