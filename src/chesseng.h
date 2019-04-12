#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "structures.c"     /* Typedefs for board, Move, etc etc */

/* ########################
 *  EXTERNAL FUNCTIONS
 * ########################
 */

//Move
int square_is_attacked(Piece *, int);


/* ########################
 *  HELPERS TO BE MOVED
 * ########################
 */

/* Does a board have the same side's piece in sq1 and sq2 */
int same_team(Piece p[], int sq1, int sq2) 
{
    return (is_black[p[sq1]] && is_black[p[sq2]]) || (is_white[p[sq1]] && is_white[p[sq2]]);
}

/* Does a board have opponent pieces in sq1 and sq2? */
bool different_team(Piece p[], int sq1, int sq2) 
{
    int s1 = p[sq1];
    int s2 = p[sq2];
    return (is_black[s1] && is_white[s2]) || (is_white[s1] && is_black[s2]);
}

/* What piece does this char refer to? */
Piece chr_to_piece(char c)
{
    switch (c) {
        case 'K': return WHITE_KING; 
        case 'Q': return WHITE_QUEEN; 
        case 'R': return WHITE_ROOK; 
        case 'B': return WHITE_BISHOP; 
        case 'N': return WHITE_KNIGHT; 
        case 'P': return WHITE_PAWN; 
        case 'k': return BLACK_KING; 
        case 'q': return BLACK_QUEEN; 
        case 'r': return BLACK_ROOK; 
        case 'b': return BLACK_BISHOP; 
        case 'n': return BLACK_KNIGHT; 
        case 'p': return BLACK_PAWN; 
        default: return NO_PIECE; 
    }
}

/* What array index does this algebraic representation of a square point to? */
int algebraic_to_sq(char file, char rank) 
{
    return (int)(file - 'a') + ((int)(rank - '0') * 8);
}

#include "fens.c"   /* Define some positions useful for testing */
#include "board.c"  /* Creating, Destroying & asking q about boards */
#include "move.c"   /* Generating legal moves */
#include "ai.c"     /* Making desicions about position quality */
