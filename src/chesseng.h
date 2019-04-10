#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

typedef enum { PLAYER_WHITE, PLAYER_BLACK } Player, Turn;

typedef enum { 
    NO_PIECE,
    BLACK_PAWN, BLACK_EP_PAWN, BLACK_CASTLING_ROOK, BLACK_ROOK, BLACK_CASTLING_KING, BLACK_KING,
    BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN,
    WHITE_PAWN, WHITE_EP_PAWN, WHITE_CASTLING_ROOK, WHITE_ROOK, WHITE_CASTLING_KING, WHITE_KING,
    WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN 
} Piece;

typedef enum {
    MAKE_RANDOM_MOVE, PROMPT_FOR_MOVE, FIND_BEST_MOVE
} PositionStrategy;

typedef enum {
    NO_S_EFFECT, EP_CAPTURE, PROMOTION, KS_CASTLE, QS_CASTLE
} MoveSideEffect;

typedef struct {
    int from;
    int to;
    Piece on_from;
    Piece on_to;
    MoveSideEffect side_effect;
} Move;

// Bundle up count of moves with the actual moves
typedef struct {
    int count;
    Move *moves;
} MoveSet;

typedef struct {
     Player turn; 
     int count;
     Piece *squares; 
    /*@out@*/ Move *moves;
} Board;

#define MYPIECE(T, P) (T? BLACK_ ## P : WHITE_ ## P)


static char FILE_MAP[] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'
};

static int RANK_MAP[] = {
    8, 8, 8, 8, 8, 8, 8, 8,
    7, 7, 7, 7, 7, 7, 7, 7,
    6, 6, 6, 6, 6, 6, 6, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    4, 4, 4, 4, 4, 4, 4, 4,
    3, 3, 3, 3, 3, 3, 3, 3,
    2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1
};

static char COLOUR_PIECE_MAP[] = { 
    '*', 
    'p', 'p', 'r', 'r', 'k', 'k', 'n', 'b', 'q', 
    'P', 'P', 'R', 'R', 'K', 'K', 'N', 'B', 'Q'
};

//static char PIECE_MAP[] = { 
//    (char)0, 
//    'P', 'P', 'R', 'R', 'K', 'K', 'N', 'B', 'Q', 
//    'P', 'P', 'R', 'R', 'K', 'K', 'N', 'B', 'Q'
//};

static int is_king[] =   {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0};
static int is_queen[] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1};
static int is_rook[] =   {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0};
static int is_bishop[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
static int is_knight[] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0};
static int is_pawn[] =   {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0};
//static int is_empty[] =  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int is_black[] =  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int is_white[] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1};

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

int algebraic_to_sq(char file, char rank) 
{
    return (int)(file - 'a') + ((int)(rank - '0') * 8);
}

#include "board.c"
#include "move.c"
#include "ai.c"
