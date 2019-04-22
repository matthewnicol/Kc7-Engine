#define Player char
#define Turn char
#define WHITE ((char)0)
#define BLACK ((char)1)
#define PLAYER_WHITE ((char)0)
#define PLAYER_BLACK ((char)1)
#define DIRECTIONWHITE ((char)-1)
#define DIRECTIONBLACK ((char)1)
#define DIRECTION(P) (P == WHITE ? DIRECTIONWHITE : DIRECTIONBLACK)
#define TURNPIECE(X, Y) ((X == WHITE && ISWHITE(Y)) || (X == BLACK && ISBLACK(Y)))

#define NO_PIECE ' '
#define BLACK_PAWN 'p'
#define BLACK_EP_PAWN 'e'
#define BLACK_CASTLING_ROOK 'w'
#define BLACK_ROOK 'r'
#define BLACK_CASTLING_KING 'o'
#define BLACK_KING 'k'
#define BLACK_KNIGHT 'n'
#define BLACK_BISHOP 'b'
#define BLACK_QUEEN 'q'
#define WHITE_PAWN 'P'
#define WHITE_EP_PAWN 'E'
#define WHITE_CASTLING_ROOK 'W'
#define WHITE_ROOK 'R'
#define WHITE_CASTLING_KING 'O'
#define WHITE_KING 'K'
#define WHITE_KNIGHT 'N'
#define WHITE_BISHOP 'B'
#define WHITE_QUEEN 'Q'
#define PIECEREP(X) (X == WHITE_CASTLING_ROOK ? WHITE_ROOK : \
         (X == WHITE_CASTLING_KING? WHITE_KING : \
         (X == BLACK_CASTLING_KING ? BLACK_KING : \
         (X == BLACK_CASTLING_ROOK? BLACK_ROOK : \
         (X == WHITE_EP_PAWN? WHITE_PAWN : \
         (X == BLACK_EP_PAWN? BLACK_PAWN : X))))))
#define Piece char

#define ISBLACK(X) (X >= 'a' && X <= 'z')
#define ISWHITE(X) (X >= 'A' && X <= 'Z')
#define ISPIECE(X) (ISBLACK(X) || ISWHITE(X))

#define ISKNIGHT(X) (X == WHITE_KNIGHT || X == BLACK_KNIGHT)
#define ISBISHOP(X) (X == WHITE_BISHOP || X == BLACK_BISHOP)
#define ISROOK(X) (X == WHITE_ROOK || X == WHITE_CASTLING_ROOK || X == BLACK_ROOK || X == BLACK_CASTLING_ROOK)
#define ISPAWN(X) (X == WHITE_PAWN || X == BLACK_PAWN || X == WHITE_EP_PAWN || X == BLACK_EP_PAWN)
#define ISKING(X) (X == WHITE_KING || X == WHITE_CASTLING_KING || X == BLACK_KING || X == BLACK_CASTLING_KING)
#define ISQUEEN(X) (X == WHITE_QUEEN || X == BLACK_QUEEN)

#define VALUEOF(X) (ISQUEEN(X) ? 9 : (ISBISHOP(X) ? 3 : (ISROOK(X) ? 5 : (ISPAWN(X) ? 1 : (ISKNIGHT(X) ? 3 : 0)))))


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
