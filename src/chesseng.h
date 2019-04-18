#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <assert.h>

//ssize_t readlink(const char *pathname, char *buf, size_t bufsiz);
/* ########################
 *  DATA STRUCTURES
 * ########################
 */

#include "structures.c"     /* Typedefs for board, Move, etc etc */

/* ########################
 *  EXTERNAL FUNCTIONS
 * ########################
 */

// piece_moves.c
static int pawn_advances(Piece*, int, Turn, Move*);
static int pawn_captures(Piece*, int, Turn, Move*);
static int pawn_ep_captures(Piece*, int, Turn, Move*);
static int pawn_promotions(Piece*, int, Turn, Move*);
static int knight_moves(Piece*, int, Move*);
static int king_moves(Piece*, int, Move*);
static int king_castles(Piece*, int, Move*);
static int linewise_piece_moves(Piece*, int, int, int, Move*);

// move.c

int square_is_attacked(Piece *, int);
/*@null@*/ static MoveSet* make_moveset(int);
static void basic_move(Move*, int, int, Piece, Piece);
static void move_with_side_effect(Move*, int, int, Piece, Piece, MoveSideEffect);
static int moves_for_square(Piece*, int, Turn, Move*);
static void remove_illegal_moves(Piece*, /*@dependent@*/ MoveSet*);
MoveSet *all_legal_moves(Piece *sq, Turn t);

// ai.c

void make_random_move(Board*, MoveSet*);
double evaluate(Piece*, MoveSet*, Player);
Move minimax_choice(Piece*, MoveSet*, Player);
double minimax(Piece*, int, Player, double, double);
void add_transposition(TranspositionTable*, int, Piece*, Turn, double);
int find_transposition(TranspositionTable*, int, Piece*, Turn, double*);

/* ########################
 *  HELPERS TO BE MOVED
 * ########################
 */

/* Does a board have the same side's piece in sq1 and sq2 */
int same_team(Piece *p, int sq1, int sq2) 
{
    assert(sq1 > -1 && sq1 < 64);
    assert(sq2 > -1 && sq2 < 64);
    return (is_black[p[sq1]] && is_black[p[sq2]]) || (is_white[p[sq1]] && is_white[p[sq2]]);
}

/* Does a board have opponent pieces in sq1 and sq2? */
bool different_team(Piece *p, int sq1, int sq2) 
{
    assert(sq1 > -1 && sq1 < 64);
    assert(sq2 > -1 && sq2 < 64);
    return (is_black[p[sq1]] && is_white[p[sq2]]) || (is_white[p[sq1]] && is_black[p[sq2]]);
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

/* ########################
 *  HELPER DEFINITIONS
 * ########################
 */

#define TOGGLE(T) (T == PLAYER_WHITE ? PLAYER_BLACK : PLAYER_WHITE)
#define EITHER(A,B,C)    ((A == B || A == C))
#define VALID(A)    (A >= 0 && A < 64)
#define COLOURCOND(C, T, W, B) (T? C == B : C == W)
#define OPPONENTS(S, A, B) ((is_white[S[A]] && is_black[S[B]]) || (is_black[S[A]] && is_white[S[B]]))
#define TOGGLE(T) (T == PLAYER_WHITE ? PLAYER_BLACK : PLAYER_WHITE)
#define MAX(A, B) (A > B ? A : B)
#define MIN(A, B) (A < B ? A : B)
#define WHITEBLACK_VAL(T, A, B) (T == PLAYER_WHITE ? A : B)
#define DEFAULT_EVAL(T) WHITEBLACK_VAL(T, -1000.00, 1000.00)

#define MYPIECE(T, P) (T? BLACK_ ## P : WHITE_ ## P)
#define THEIRPIECE(T, P) (T? WHITE_ ## P : BLACK_ ## P)

/* ########################
 *  OUR CODE
 * ########################
 */

#include "fens.c"        /* Define some positions useful for testing */
#include "board.c"       /* Creating, Destroying & asking q about boards */
#include "move.c"        /* Generating legal moves */
#include "ai.c"          /* Making desicions about position quality */
#include "piece_moves.c" /* Generation functions for squares pieces can move w/o validation */
#include "pgn.c"
