#include "chesseng.h"

int main ()
{
    int i;
    Board *b = new_board();
    assert (b != NULL);
    
    // dumb call in here so my compiler doesn't yell at me for having an unused function
    FEN(FENS[0], b);

    standard_position(b);
    
    for (i = 0; i < 20; i++) {
        printBoard(b->squares);
        MoveSet *m = all_legal_moves(b->squares, b->turn);
        Move mm = minimax_choice(b->squares, m, b->turn);
        apply_move(b->squares, &mm);
        free(m->moves);
        free(m);
        b->turn = !b->turn;
    }

    free(b->squares);
    free(b->moves);
    free(b);
    exit(0);
}

