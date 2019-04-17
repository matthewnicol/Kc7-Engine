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
    int is_checking_move; // Store to save on double-handling
} Move;

typedef struct {
    double evaluation;
    int depth;
    Piece *squares;
    Player turn;
} Transposition;

typedef struct {
    Transposition *transposition;
    int transpositions_counted;
    int max_transpositions;
} TranspositionTable;

#define LATEST_TRANSP(T) (T->transposition + T->transpositions_counted)


// Bundle up count of moves with the actual moves
typedef struct {
    int count;
    int king_pos;
    Move *moves;
} MoveSet;

typedef struct {
     Player turn; 
     int count;
     Piece *squares; 
    /*@out@*/ Move *moves;
} Board;

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

static double PIECE_VALUE_MAP[] = { 
    0, 
    -1.0, -1.0, -5.0, -5.0, 0.0, 0.0, -3, -3.0, -9.0, 
    1.0, 1.0, 5.0, 5.0, 0.0, 0.0, 3.0, 3.0, 9.0
};

static int is_king[] =   {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0};
static int is_queen[] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1};
static int is_rook[] =   {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0};
static int is_bishop[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
static int is_knight[] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0};
static int is_pawn[] =   {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0};
//static int is_empty[] =  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int is_black[] =  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int is_white[] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1};
