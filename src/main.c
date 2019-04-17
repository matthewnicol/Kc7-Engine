#include "chesseng.h"
#include <time.h>

int main ()
{
    int i;
    Board *b = new_board();
    assert (b != NULL);
    standard_position(b);
    FEN(FENS[3], b);
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    for (i = 0; i < 20; i++) {
        printBoard(b->squares);
        MoveSet *m = all_legal_moves(b->squares, b->turn);
        if (is_checkmate(b->squares, m)) {
            printf("Checkmate!\n");
            free(m->moves);
            free(m);
            break;
        }
        else if (is_stalemate(b->squares, m)) {
            printf("Stalemate!\n");
            free(m->moves);
            free(m);
            break;
        }
        Move mm = minimax_choice(b->squares, m, b->turn);
        printMove(-1, &mm);
        apply_move(b->squares, &mm);
        free(m->moves);
        free(m);
        b->turn = !b->turn;
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken to solve: %f\n", cpu_time_used);

    free(b->squares);
    free(b->moves);
    free(b);
    exit(0);
}

