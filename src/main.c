#include <stdio.h>
#include "board.c"
#include "../include/board.h"

int main(char* argc, char**argv){


        printf("Do you want to start a new game? y/n: \n");
        char response;
        scanf("%c", &response);

        if(response == 'y' || response == 'Y'){
            initBoard();

        }else if(response == 'n' || response == 'N'){

            printf("\nThank you for playing!");

        }else{

            printf("\nPlease enter a valid response");

        }

            return 0;
}

// initGame(){

// }

// gameLoop(){
    
// }