#include <stdio.h>

#include "../include/board.h"
#include "../include/game.h"

void startGame(){
    char current = 'A';
    int r1, c1, r2, c2;

    initBoard();

    while(!isGameOver()){
        printBoard();
        printf("Player %c turn\n", current);
        
        printf("Enter (r1 c1): ");
        scanf("%d %d", &r1, &c1);
        
        printf("Enter (r2 c2): ");
        scanf("%d %d", &r2, &c2);

        int result = drawLine(r1, c1, r2, c2, current);

        if(result == -1){
            printf("Invalid move. Try again.\n");
            continue;
        }

        // if no box → switch player
        if(result == 0){
            current = (current == 'A') ? 'B' : 'A';
        }
    }

    printBoard();

    printf("Game Over!\n");
    printf("A: %d | B: %d\n", getScoreA(), getScoreB());

    if(getScoreA() > getScoreB()) printf("Player A wins!\n");
    else if(getScoreB() > getScoreA()) printf("Player B wins!\n");
    else printf("It's a draw!\n");
}