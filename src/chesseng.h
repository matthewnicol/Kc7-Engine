#include "definitions.c"

int isin(int t, int l[])
{
    int i, matches = 0;
    for (i = 0; l[i] != t || (matches = 1); i++);
    return matches;
}

PieceMovement *blankPieceMovement(void);
PieceMovement *makePieceMovement(int from, int to, Piece on_from, Piece on_to);
Move *makeMove(PieceMovement *main, PieceMovement *alt);
int same_team(Board*, int, int);

#include "devel.c"
#include "pawn.c"
#include "knight.c"
#include "bishop.c"
#include "rook.c"
#include "queen.c"
#include "king.c"
#include "move.c"
