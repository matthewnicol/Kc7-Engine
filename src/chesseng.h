#include "definitions.c"

int tuple_matches(char a, char b, char l[][2])
{
    int i;
    for (i = 0; !(l[i][0] == '\0' && l[i][1] == '\0') && (l[i][0] != a && l[i][1] != b); i++);
    return l[i][0] == a && l[i][1] == b;

}

// Board
void reverseMove(Board*, Move*);
int can_opponent_attack(Board*, int);
Board get_board(Turn t);
void apply_move (Board *b, Move *m);
void reverse_move(Board *b, Move *m);
MoveSet trim_invalid_moves(Board*, MoveSet);
void standard (Board *b);
Board* copy_board(Board*);
void applyMove(Board*, Move*); 
PieceMovement *blankPieceMovement(void);
PieceMovement *makePieceMovement(int, int, Piece, Piece);
Move *makeMove(PieceMovement*, PieceMovement*);
Move *makeSimpleMove(int, int, Piece, Piece);
int same_team(Piece[64], int, int);

#include "board.c"
#include "devel.c"
#include "pieces/pieces.h"
#include "move.c"
#include "position.c"
