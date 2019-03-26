#include "definitions.c"

int isin(int t, int l[])
{
    int i, matches = 0;
    for (i = 0; l[i] != t || (matches = 1); i++);
    return matches;
}

// Board
void reverseMove(Board*, Move*);
int can_opponent_attack(Board*, int);
Board get_board(Turn t);
void apply_move (Board *b, Move *m);
void reverse_move(Board *b, Move *m);
int trim_invalid_positions(Board*, Move**, int);
void standard (Board *b);
Board* copy_board(Board*);
void applyMove(Board*, Move*); 
PieceMovement *blankPieceMovement(void);
PieceMovement *makePieceMovement(int, int, Piece, Piece);
Move *makeMove(PieceMovement*, PieceMovement*);
int same_team(Board*, int, int);

#include "board.c"
#include "devel.c"
#include "pieces/pieces.h"
#include "move.c"
