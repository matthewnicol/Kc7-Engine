#include "chesseng.h"
#include <time.h>

int main ()
{
#if 0
    Board *b = new_board();
    assert (b != NULL);
    standard_position(b);
//    FEN(FENS[3], b);
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    //for (int i = 0; i < 20; i++) {
    while(1) {
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
#endif
    struct dirent **dirp;
    FILE *fp;
//    char *buffer = malloc(sizeof(char)*150);
    char *pgnpath = malloc(sizeof(char)*150);
    char *filepath = malloc(sizeof(char)*150);
//    readlink("/proc/self/exe", buffer, BUFSIZ);
    sprintf(pgnpath, "%s", "/home/nicol/dev/chesseng/pgn/");

    int pgn_count = scandir(pgnpath, &dirp, NULL, alphasort);
    while (--pgn_count > 0) {
        sprintf(filepath, "%s%s", pgnpath, dirp[pgn_count]->d_name); 
        printf("%s\n", filepath);
        fp = fopen(filepath, "r");
        load_pgn(fp);
        fclose(fp);
        exit(0);
    }

//    free(buffer);
    free(pgnpath);
    free(filepath);
    free(dirp);
    exit(0);
}

