#include "chesseng.h"

int main ()
{
    int i;
    Board *b = new_board();
    assert (b != NULL);
    
    // dumb call in here so my compiler doesn't yell at me for having an unused function

    standard_position(b);
    FEN(FENS[0], b);
    
    for (i = 0; i < 20; i++) {
        printBoard(b->squares);
        MoveSet *m = all_legal_moves(b->squares, b->turn);
        if (is_checkmate(b->squares, m)) {
            printf("Checkmate!\n");
            break;
        }
        else if (is_stalemate(b->squares, m)) {
            printf("Stalemate!\n");
            break;
        }
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

