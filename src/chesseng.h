#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
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
void get_board(Board*);
void apply_move (Board *b, Move *m);
void reverse_move(Board *b, Move *m);
MoveSet trim_invalid_moves(Board*, MoveSet);
void standard (Board *b);
void copy_board(Board*, Board*);
void applyMove(Board*, Move*); 
PieceMovement *blankPieceMovement(void);
PieceMovement *makePieceMovement(int, int, Piece, Piece);
Move *makeMove(PieceMovement*, PieceMovement*);
Move *makeSimpleMove(int, int, Piece, Piece);
int castling_available(Piece[], int, Turn);

#include "board.c"
#include "devel.c"
#include "pieces/pieces.h"
#include "move.c"
#include "position.c"
