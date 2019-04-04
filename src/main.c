#include "chesseng.h"

/* Test configuration */
int main ()
{
    time_t t;
    
    srand((unsigned) time(&t));
    int i;
    Move moves[20];
    Move **m;
    m = &moves;
    Board b = get_board(m);
    FEN(&b, "2rr2k1/pp2qp1p/1Nn1pp2/1N6/3P4/P6P/1P2QPP1/2R2RK1 b - -");
    for (i = 0; i < 20; i++) {
        printBoard(&b);
        handle_position(MAKE_RANDOM_MOVE, &b);
    }
    exit(0);
}

