/* Functions for creating and manipulating boards */

/* Get standard board configuration */

void standard (Board *b) 
{
    int i;
    for (i = 0; i < 8; i++) {
        b->piecemap[i+8] = BLACK_PAWN;
        b->piecemap[63-8-i] = WHITE_PAWN;
    }
    b->piecemap[0] = b->piecemap[7] = BLACK_CASTLING_ROOK;
    b->piecemap[1] = b->piecemap[6] = BLACK_KNIGHT;
    b->piecemap[2] = b->piecemap[5] = BLACK_BISHOP;
    b->piecemap[63-0] = b->piecemap[63-7] = WHITE_CASTLING_ROOK;
    b->piecemap[63-1] = b->piecemap[63-6] = WHITE_KNIGHT;
    b->piecemap[63-2] = b->piecemap[63-5] = WHITE_BISHOP;
    b->piecemap[4] = BLACK_QUEEN;
    b->piecemap[59] = WHITE_QUEEN;
    b->piecemap[3] = BLACK_CASTLING_KING;
    b->piecemap[60] = WHITE_CASTLING_KING;
}

/* Make and return a new board. */

Board get_board()
{
    Board *b = malloc(sizeof(Board) + sizeof(Move[20]));

    int i;
    b->turn = PLAYER_WHITE;
    b->cur_move = 0;

    for (i = 0; i < 64; i++) {
        b->piecemap[i] = NO_PIECE;
    }
    
    return *b;
}

/* Duplicate an existing board. */

Board *copy_board(Board *b)
{
    Board *b2 = malloc(sizeof(Board));
    int i;
    for (i = 0; i < 64; i++) b2->piecemap[i] = b->piecemap[i];
    b2->turn = b->turn;
    return b2;
}

#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#define SETSQ(P)      b->piecemap[sq++] = P

/* Build a board from a FEN representation */

void FEN(Board *b, char *fen) 
{
    char cur;
    int i, sq = 0;

    for (i = 0; fen[i] != ' '; i++) {
        cur = fen[i];
        switch (cur) {
            case '8': SETSQ(NO_PIECE);
            case '7': SETSQ(NO_PIECE); 
            case '6': SETSQ(NO_PIECE); 
            case '5': SETSQ(NO_PIECE); 
            case '4': SETSQ(NO_PIECE); 
            case '3': SETSQ(NO_PIECE); 
            case '2': SETSQ(NO_PIECE); 
            case '1': SETSQ(NO_PIECE); break;
            case 'p': SETSQ(BLACK_PAWN); break;
            case 'P': SETSQ(WHITE_PAWN); break;
            case 'n': SETSQ(BLACK_KNIGHT); break;
            case 'N': SETSQ(WHITE_KNIGHT); break;
            case 'b': SETSQ(BLACK_BISHOP); break;
            case 'B': SETSQ(WHITE_BISHOP); break;
            case 'r': SETSQ(BLACK_ROOK); break;
            case 'R': SETSQ(WHITE_ROOK); break;
            case 'q': SETSQ(BLACK_QUEEN); break;
            case 'Q': SETSQ(WHITE_QUEEN); break;
            case 'k': SETSQ(BLACK_KING); break;
            case 'K': SETSQ(WHITE_KING); break;
            case '/': break;
        }
    }
    b->turn = ((char)fen[++i] == 'b') ? PLAYER_BLACK : PLAYER_WHITE;

    while ((cur = (char)fen[++i]) != ' ') {
        if (cur == 'K') {
            b->piecemap[60] = WHITE_CASTLING_KING;
            b->piecemap[63] = WHITE_CASTLING_ROOK;
        } else if (cur == 'Q') {
            b->piecemap[60] = WHITE_CASTLING_KING;
            b->piecemap[56] = WHITE_CASTLING_ROOK;
        } else if (cur == 'k') {
            b->piecemap[3] = BLACK_CASTLING_KING;
            b->piecemap[7] = BLACK_CASTLING_ROOK;
        } else if (cur == 'q') {
            b->piecemap[3] = BLACK_CASTLING_KING;
            b->piecemap[0] = BLACK_CASTLING_ROOK;
        }
    }
}
