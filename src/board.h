/* =========================
   board.h
   ========================= */

#ifndef BOARD_H
#define BOARD_H

#define ROWS 5
#define COLS 6

extern char hLines[ROWS][COLS-1];
extern char vLines[ROWS-1][COLS];
extern char boxes[ROWS-1][COLS-1];

void initBoard();
void printBoard();

int drawLine(int r1, int c1, int r2, int c2, char player);

int checkBox(int r, int c, char player);

int isGameOver();

#endif