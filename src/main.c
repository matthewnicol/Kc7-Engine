#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "chesseng.h"

/* Test configuration */
int main ()
{
    int i;
    Board b = get_board(PLAYER_WHITE); 
    standard(&b);
    printf("Turn %d\n", b.turn);
    for (i = 0; i < 20; i++) {
        printBoard(&b);
        sleep(2);
        valid_moves(&b);
    }
    exit(0);
}

