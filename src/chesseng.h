#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

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

int square_is_attacked(Board *, int);
/*@null@*/ static MoveSet* make_moveset(int);
static void basic_move(Move*, int, int);
static void move_with_side_effect(Move*, int, int, MoveSideEffect);
static int moves_for_square(Piece*, int, Turn, Move*);
static void remove_illegal_moves(Board*, /*@dependent@*/ MoveSet*);
MoveSet *all_legal_moves(Board *, Turn);

// ai.c

void make_random_move(Board*, MoveSet*);
double evaluate(Board*, MoveSet*, Player);
Move minimax_choice(Board*, MoveSet*, Player);
double minimax(Board*, int, Player, double, double);
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
    return (ISBLACK(p[sq1]) && ISBLACK(p[sq2])) || (ISWHITE(p[sq1]) && ISWHITE(p[sq2]));
}

/* Does a board have opponent pieces in sq1 and sq2? */
bool different_team(Piece *p, int sq1, int sq2) 
{
    assert(sq1 > -1 && sq1 < 64);
    assert(sq2 > -1 && sq2 < 64);
    return (ISBLACK(p[sq1]) && ISWHITE(p[sq2])) || (ISWHITE(p[sq1]) && ISBLACK(p[sq2]));
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

int matches_pair(char a, char b, char pairs[][2])
{
    for (int i = 0; pairs[i][0] != '\0' && pairs[i][1] != '\0'; i++) {
        if (pairs[i][0] == a && pairs[i][0] == b) return 1;
    }
    return 0;
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
#define EITHER3(A,B,C,D)    ((A == B || A == C || A == D))
#define VALID(A)    (A >= 0 && A < 64)
#define COLOURCOND(C, T, W, B) (T? C == B : C == W)
#define FRIENDS(S, A, B) ((ISWHITE(S[A]) && ISWHITE(S[B])) || (ISBLACK(S[A]) && ISBLACK(S[B])))
#define ENEMIES(S, A, B) ((ISWHITE(S[A]) && ISBLACK(S[B])) || (ISBLACK(S[A]) && ISWHITE(S[B])))
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
