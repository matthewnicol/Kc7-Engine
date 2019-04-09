#include "chesseng.h"

int main ()
{
    int i;
    Board *b = new_board();
    assert (b != NULL);
    FEN("4rr2/pb1n1p1k/1p2P1pp/2pP4/2P3nq/3B2N1/PB1Q2PP/4RRK1 w - -", b);
    standard_position(b);
    for (i = 0; i < 20; i++) {
        printBoard(b->squares);
        MoveSet *m = all_legal_moves(b->squares, b->turn);
        make_random_move(b, m);
        free(m->moves);
        free(m);
        b->turn = !b->turn;
        //if ((char)getchar() == 'q') exit(0);
    }

    free(b->squares);
    free(b);
    exit(0);
}

