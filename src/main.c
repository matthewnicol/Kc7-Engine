#include "chesseng.h"
#include <time.h>

int main ()
{
    Board *b = new_board();
    assert (b != NULL);
    FEN(FENS[3], b);
    printf("Position hash: %i", b->hash);
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    //for (int i = 0; i < 4; i++) {
    while(1) {
        printBoard(b->squares);
        MoveSet *m = all_legal_moves(b, b->turn);
        printAllMoves(m);
        if (is_checkmate(b, m)) {
            printf("Checkmate!\n");
            free(m->moves);
            free(m);
            break;
        }
        else if (is_stalemate(b, m)) {
            printf("Stalemate!\n");
            free(m->moves);
            free(m);
            break;
        }
        Move mm = minimax_choice(b, m, b->turn);
        printMove(-1, &mm);
        apply_move(b, &mm);
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

