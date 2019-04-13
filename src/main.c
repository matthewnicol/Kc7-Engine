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
        if (b->turn == PLAYER_WHITE) {
            Move *mm = malloc(sizeof(Move));
            b->turn = !b->turn;
            minmax(b, 2, mm);
            b->turn = !b->turn;
            apply_move(b->squares, mm);
            free(mm);
        } else {
            Move *mm = malloc(sizeof(Move));
            b->turn = !b->turn;
            minmax(b, 2, mm);
            b->turn = !b->turn;
            apply_move(b->squares, mm);
            free(mm);
        }
        printBoard(b->squares);
//        int ddd;
//        scanf("%d", &ddd);
//        if (ddd == 3) {
//            free(m->moves);
//            free(m);
//            free(b->squares);
//            free(b);
//            exit(0);
//        }
        free(m->moves);
        free(m);
        b->turn = !b->turn;
    }

    free(b->squares);
    free(b->moves);
    free(b);
    exit(0);
}

