typedef enum { PLAYER_NONE, PLAYER_WHITE, PLAYER_BLACK } Player, Turn;

typedef enum { 
    NO_PIECE,
    BLACK_PAWN, BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN, BLACK_KING, BLACK_EP_PAWN, 
    WHITE_PAWN, WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN, WHITE_KING, WHITE_EP_PAWN
} Piece;

typedef struct {
    int from;       /* 0-64 -> The square we moved from */
    int to;         /* 0-64 -> The square we moved to */
    Piece on_from;  /* Record pieces on each square to help with reversing board */
    Piece on_to;    /* Record pieces on each square to help with reversing board */
} PieceMovement;

typedef struct {
    PieceMovement *main; /* The move we want to make */
    PieceMovement *alt;  /* Side effects of this move, i.e. castling, e.p. */
} Move;

typedef struct {
    Piece piecemap[64]; 
    Player turn; 
    int cur_move; 
    Move *moves[];
} Board;

#define RANK1(x)   (x < 64 && x > 56)
#define RANK2(x)   (x < 56 && x > 48)
#define RANK3(x)   (x < 48 && x > 40)
#define RANK4(x)   (x < 40 && x > 32)
#define RANK5(x)   (x < 32 && x > 24)
#define RANK6(x)   (x < 24 && x > 16)
#define RANK7(x)   (x < 16 && x > 8)
#define RANK8(x)   (x < 8 && x > 0)
#define RANK(x)    (x / 8 + 1)
#define NEXTRANK(x)        (x + 8)
#define PREVRANK(x)        (x - 8)
#define OCCUPIED(A, B)     (A->piecemap[B] != NO_PIECE)
#define ONSQ(A, B, C)      (A->piecemap[B] == C)   
#define SAMERANK(A, B)    (RANK(A) == RANK(B))
#define PAWN(X)           (X == WHITE_PAWN || X == BLACK_PAWN || X == WHITE_EP_PAWN || X == BLACK_EP_PAWN)
#define KNIGHT(X)           (X == WHITE_KNIGHT || X == BLACK_KNIGHT)
#define BISHOP(X)           (X == WHITE_BISHOP || X == BLACK_BISHOP)
#define ROOK(X)           (X == WHITE_ROOK || X == BLACK_ROOK)
#define QUEEN(X)           (X == WHITE_QUEEN || X == BLACK_QUEEN)
#define KING(X)           (X == WHITE_KING || X == BLACK_KING)
#define VALID(X)         (X >= 0 && X < 64)

static char *FILE_MAP[] = {
    "a", "b", "c", "d", "e", "f", "g", "h",
    "a", "b", "c", "d", "e", "f", "g", "h",
    "a", "b", "c", "d", "e", "f", "g", "h",
    "a", "b", "c", "d", "e", "f", "g", "h",
    "a", "b", "c", "d", "e", "f", "g", "h",
    "a", "b", "c", "d", "e", "f", "g", "h",
    "a", "b", "c", "d", "e", "f", "g", "h",
    "a", "b", "c", "d", "e", "f", "g", "h"
};

static char *RANK_MAP_S[] = {
    "8", "8", "8", "8", "8", "8", "8", "8",
    "7", "7", "7", "7", "7", "7", "7", "7",
    "6", "6", "6", "6", "6", "6", "6", "6",
    "5", "5", "5", "5", "5", "5", "5", "5",
    "4", "4", "4", "4", "4", "4", "4", "4",
    "3", "3", "3", "3", "3", "3", "3", "3",
    "2", "2", "2", "2", "2", "2", "2", "2",
    "1", "1", "1", "1", "1", "1", "1", "1",
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

static char *COLOUR_PIECE_MAP[] = { "*", "P", "N", "B", "R", "Q", "K", "P", "p", "n", "b", "r", "q", "k", "p" };

static char *TEXT_PIECE_MAP[] = { "", "", "N", "B", "R", "Q", "K", "", "", "N", "B", "R", "Q", "K", "" };
static char *PIECE_MAP[] = { "", "P", "N", "B", "R", "Q", "K", "P", "P", "N", "B", "R", "Q", "K", "" };

static int is_king[] =   {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0};
static int is_queen[] =  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0};
static int is_rook[] =   {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
static int is_bishop[] = {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
static int is_knight[] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0};
static int is_pawn[] =   {0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1};
static int is_empty[] =  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int is_black[] =  {0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0};
static int is_white[] =  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1};

/* Does a board have the same side's piece in sq1 and sq2 */
int same_team(Board *b, int sq1, int sq2) {
    return (is_black[b->piecemap[sq1]] && is_black[b->piecemap[sq2]]) ||
        (is_white[b->piecemap[sq1]] && is_white[b->piecemap[sq2]]);
}
