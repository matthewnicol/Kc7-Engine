typedef enum { PLAYER_WHITE, PLAYER_BLACK } Player, Turn;

typedef enum { 
    NO_PIECE,
    BLACK_PAWN, BLACK_EP_PAWN, BLACK_CASTLING_ROOK, BLACK_ROOK, BLACK_CASTLING_KING, BLACK_KING,
    BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN,
    WHITE_PAWN, WHITE_EP_PAWN, WHITE_CASTLING_ROOK, WHITE_ROOK, WHITE_CASTLING_KING, WHITE_KING,
    WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN 
} Piece;

typedef Piece PieceMap[64];

typedef enum {
    MAKE_RANDOM_MOVE, PROMPT_FOR_MOVE, FIND_BEST_MOVE
} PositionStrategy;

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

// Bundle up count of moves with the actual moves
typedef struct {
    Move **moves;
    int count;
} MoveSet;


#define VALID(X)         (X >= 0 && X < 64)

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

static char PIECE_MAP[] = { 
    0, 
    'P', 'P', 'R', 'R', 'K', 'K', 'N', 'B', 'Q', 
    'P', 'P', 'R', 'R', 'K', 'K', 'N', 'B', 'Q'
};

static int is_king[] =   {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0};
//static int is_queen[] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1};
//static int is_rook[] =   {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0};
//static int is_bishop[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
//static int is_knight[] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0};
static int is_pawn[] =   {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0};
//static int is_empty[] =  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int is_black[] =  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int is_white[] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1};

/* Does a board have the same side's piece in sq1 and sq2 */
int same_team(Piece p[64], int sq1, int sq2) 
{
    return (is_black[p[sq1]] && is_black[p[sq2]]) || (is_white[p[sq1]] && is_white[p[sq2]]);
}

/* Does a board have opponent pieces in sq1 and sq2? */
int different_team(Piece p[64], int sq1, int sq2) 
{
    return (is_black[p[sq1]] && is_white[p[sq2]]) || (is_white[p[sq1]] && is_black[p[sq2]]);
}
