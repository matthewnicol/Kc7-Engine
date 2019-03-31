#include "chesseng.h"

/* Test configuration */
int main ()
{
    time_t t;
    
    srand((unsigned) time(&t));
    int i;
    Board b = get_board(); 
    standard(&b);
    printf("Turn %d\n", b.turn);
    for (i = 0; i < 20; i++) {
        printBoard(&b);
        handle_position(MAKE_RANDOM_MOVE, &b);
    }
    exit(0);
}

