#include <stdlib.h>
#include <time.h>
#include "board.h"

// function from game.c
void startGame();

int main(){
    // initialize random seed (needed for bot randomness)
    srand(time(NULL));

    // initialize the board
    initBoard();

    // start the game (handles mode selection + gameplay)
    startGame();

    return 0;
}