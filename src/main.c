#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include "chesseng.h"

Board get_board(Turn t);
void standard ();
void apply_move (Board *b, Move *m);
void reverse_move(Board *b, Move *m);

/* Test configuration */
int main ()
{
    Board b = get_board(PLAYER_WHITE); 
    standard(&b);
    int i;
    printf("Turn %d\n", b.turn);
    printBoard(&b);
    validMoves(&b);
    exit(0);
}

/* Get standard board configuration */
void standard (Board *b) 
{
    int i;
    for (i = 0; i < 8; i++) {
        b->piecemap[i+8] = BLACK_PAWN;
        b->piecemap[63-8-i] = WHITE_PAWN;
    }
    b->piecemap[0] = b->piecemap[7] = BLACK_ROOK;
    b->piecemap[1] = b->piecemap[6] = BLACK_KNIGHT;
    b->piecemap[2] = b->piecemap[5] = BLACK_BISHOP;
    b->piecemap[63-0] = b->piecemap[63-7] = WHITE_ROOK;
    b->piecemap[63-1] = b->piecemap[63-6] = WHITE_KNIGHT;
    b->piecemap[63-2] = b->piecemap[63-5] = WHITE_BISHOP;
    b->piecemap[4] = BLACK_QUEEN;
    b->piecemap[59] = WHITE_QUEEN;
    b->piecemap[3] = BLACK_KING;
    b->piecemap[60] = WHITE_KING;
}

Board get_board(Turn t)
{
    Board *b = malloc(sizeof(Board) + sizeof(Move[20]));

    int i;
    b->turn = PLAYER_WHITE;
    b->cur_move = 0;

    for (i = 0; i < 64; i++) {
        b->piecemap[i] = NO_PIECE;
    }
    
    return *b;
}

