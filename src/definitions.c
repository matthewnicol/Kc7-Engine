typedef enum { PLAYER_WHITE, PLAYER_BLACK } Player, Turn;

typedef enum { 
    NO_PIECE,
    BLACK_PAWN, BLACK_EP_PAWN, BLACK_STILL_ROOK, BLACK_MOVED_ROOK, BLACK_STILL_KING, BLACK_MOVED_KING,
    BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN,
    WHITE_PAWN, WHITE_EP_PAWN, WHITE_STILL_ROOK, WHITE_MOVED_ROOK, WHITE_STILL_KING, WHITE_MOVED_KING,
    WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING 
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

static char RANK_MAP_S[] = {
    '8', '8', '8', '8', '8', '8', '8', '8',
    '7', '7', '7', '7', '7', '7', '7', '7',
    '6', '6', '6', '6', '6', '6', '6', '6',
    '5', '5', '5', '5', '5', '5', '5', '5',
    '4', '4', '4', '4', '4', '4', '4', '4',
    '3', '3', '3', '3', '3', '3', '3', '3',
    '2', '2', '2', '2', '2', '2', '2', '2',
    '1', '1', '1', '1', '1', '1', '1', '1',
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
    'P', 'P', 'R', 'R', 'K', 'K', 'N', 'B', 'Q', 
    'p', 'p', 'r', 'r', 'k', 'k', 'n', 'b', 'q'
};

static char TEXT_PIECE_MAP[] = { 
    0, 
    0, 0, 'R', 'R', 'K', 'K', 'N', 'B', 'Q', 
    0, 0, 'R', 'R', 'K', 'K', 'N', 'B', 'Q'
};

static char PIECE_MAP[] = { 
    0, 
    'P', 'P', 'R', 'R', 'K', 'K', 'N', 'B', 'Q', 
    'P', 'P', 'R', 'R', 'K', 'K', 'N', 'B', 'Q'
};

static int is_king[] =   {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0};
static int is_queen[] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1};
static int is_rook[] =   {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0};
static int is_bishop[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
static int is_knight[] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0};
static int is_pawn[] =   {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0};
static int is_empty[] =  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int is_black[] =  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int is_white[] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1};

/* Does a board have the same side's piece in sq1 and sq2 */
int same_team(Piece p[64], int sq1, int sq2) {
    return (is_black[p[sq1]] && is_black[p[sq2]]) || (is_white[p[sq1]] && is_white[p[sq2]]);
}
