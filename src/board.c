/*@null@*/ Board *new_board();
/*@null@*/ Board *copy_board(Board *);

Board *new_board() {
    Board *b = malloc(sizeof(Board));
    if (b == NULL) return NULL;

    b->squares = malloc(sizeof(Piece)*64);
    if (b->squares == NULL) {
        free(b);
        return NULL;
    }
    b->moves = malloc(sizeof(Move));
    if (b->moves == NULL) {
        free(b->squares);
        free(b);
        return NULL;
    }
    b->turn = PLAYER_WHITE;
    b->count = 0;
    for (int i = 0; i < 64; i++) b->squares[i]= NO_PIECE;
    return b;
}

Board *copy_board(Board *b)
{
    int i;
    Board *bcopy = malloc(sizeof(Board));
    if (bcopy == NULL) {
        return NULL;
    }
    bcopy->squares = malloc(sizeof(Piece)*64);
    if (bcopy->squares == NULL) {
        free(bcopy);
        return NULL;
    }
    for (i = 0; i < 64; i++) {
        bcopy->squares[i] = b->squares[i];
    }
    bcopy->turn = b->turn;
    bcopy->count = b->count;
    return bcopy;
}

int is_checkmate(Piece *sq, MoveSet *m)
{
   return m->count == 0 && square_is_attacked(sq, m->king_pos);
}

int is_stalemate(Piece *sq, MoveSet *m)
{
   return m->count == 0 && !square_is_attacked(sq, m->king_pos);
}

int is_check(Piece *sq, Player checked)
{
    for (int i = 0; i < 64; i++) {
        if (sq[i] == MYPIECE(checked, KING) || sq[i] == MYPIECE(checked, CASTLING_KING)) {
            return square_is_attacked(sq, i);
        }
    }
    return -1;

}


void FEN(char *fen, Board *b) 
{
    int i, sq = 0;

    // Where the pieces are
    for (i = 0; fen[i] != ' '; i++) {
        if (fen[i] == '/') continue;
        if (fen[i] > '0' && fen[i] < '9') {
            int k = (int)(fen[i] - '0');
            while (k-- > 0) {
                b->squares[sq++] = NO_PIECE;
                printf("%i NO_PIECE\n", sq-1);
            }
        } else {
            printf("%i %c\n", sq, fen[i]);
            b->squares[sq++] = chr_to_piece(fen[i]);
        }
    }
    
    // Whose turn it is in the current position
    b->turn = (fen[++i] == 'w' ? PLAYER_WHITE : PLAYER_BLACK);

    // Kingside and Queenside castling rights
    for (i+=2; fen[i] != ' '; i++) {
        if (fen[i] == 'q') {
            assert(EITHER(b->squares[4], BLACK_KING, BLACK_CASTLING_KING));
            b->squares[4] = BLACK_CASTLING_KING;
            b->squares[7] = BLACK_CASTLING_ROOK;
        }
        if (fen[i] == 'k') {
            assert(EITHER(b->squares[4], BLACK_KING, BLACK_CASTLING_KING));
            b->squares[4] = BLACK_CASTLING_KING;
            b->squares[0] = BLACK_CASTLING_ROOK;
        }
        if (fen[i] == 'K') {
            assert(EITHER(b->squares[60], WHITE_KING, WHITE_CASTLING_KING));
            b->squares[60] = WHITE_CASTLING_KING;
            b->squares[63] = WHITE_CASTLING_ROOK;
        }
        if (fen[i] == 'Q') {
            assert(EITHER(b->squares[60], WHITE_KING, WHITE_CASTLING_KING));
            b->squares[60] = WHITE_CASTLING_KING;
            b->squares[56] = WHITE_CASTLING_ROOK;
        }
    }
    
    // En Passant Square
    if (fen[++i] != '-') {
        Piece np = b->squares[sq] == WHITE_PAWN? WHITE_EP_PAWN : BLACK_EP_PAWN;
        b->squares[sq] = np;
    }
}

void standard_position(Board *b)
{ 
    FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", b);
}

void apply_move(Piece *sq, Move *m)
{
    sq[m->to] = m->on_from;
    sq[m->from] = NO_PIECE;
    if (m->side_effect == KS_CASTLE) {
        sq[m->from+1] = sq[m->to] == WHITE_CASTLING_KING ? WHITE_ROOK : BLACK_ROOK;
        sq[m->to+1] = NO_PIECE;
    } else if (m->side_effect == QS_CASTLE) {
        sq[m->from-2] = sq[m->to] == WHITE_CASTLING_KING ? WHITE_ROOK : BLACK_ROOK;
        sq[m->to+4] = NO_PIECE;
    } else if (m->side_effect == EP_CAPTURE) {
        sq[m->to + (8 * (is_white[sq[m->to]] ? 1 : -1))] = NO_PIECE;
    } else if (m->side_effect == PROMOTION) {
        // this side effect is needed for reverse_move
    }
}

void reverse_move(Piece *sq, Move *m)
{
    sq[m->from] = m->on_from;
    sq[m->to] = m->on_to;
    if (m->side_effect == KS_CASTLE) {
        sq[m->from] = sq[m->to] == WHITE_KING ? WHITE_CASTLING_KING : BLACK_CASTLING_KING;
        sq[m->to+1] = sq[m->from] == WHITE_CASTLING_KING? WHITE_CASTLING_ROOK : BLACK_CASTLING_ROOK;
        sq[m->to-1] = NO_PIECE;
    } else if (m->side_effect == QS_CASTLE) {
        sq[m->from] = sq[m->to] == WHITE_KING ? WHITE_CASTLING_KING : BLACK_CASTLING_KING;
        sq[m->from-2] = sq[m->to] == WHITE_CASTLING_KING ? WHITE_CASTLING_ROOK : BLACK_CASTLING_ROOK;
        sq[m->to+1] = NO_PIECE;
    } else if (m->side_effect == EP_CAPTURE) {
        sq[m->to + (8 * (is_white[sq[m->to]] ? 1 : -1))] = sq[m->from] == WHITE_PAWN ? BLACK_EP_PAWN : WHITE_EP_PAWN;
    } else if (m->side_effect == PROMOTION) {
        // this side effect is needed for reverse_move
    }
}

#define RP(X)   (COLOUR_PIECE_MAP[p[X]])

void printBoard(Piece p[])
{
    int j = 0, i = 0;
    char *reset = "\033[0m";
    char *black = "\033[1;31m";
    char *white = "\033[1;97m";
    char *sqnum = "\033[90m";
    printf("\n");
    for (i = 0; i < 8; i++) {
       
        printf("\t|");
        for (j = 0; j < 8; j++) {
            printf("%s%02i---|%s", sqnum, i*8+j, reset);
        }
        printf("\n");
        printf("\t%s|%s", sqnum, reset);
        for (j = 0; j < 8; j++) {
            printf("%s%4c%s %s|%s", 
                is_black[p[i*8+j]]? black : white,
                p[i*8+j]? RP(i*8+j) : ' ',
                reset,
                sqnum,
                reset
            );
        }
        printf("\n");
    }
    printf("\t|");
    for (j = 0; j < 8; j++) {
        printf("%s-----|%s", sqnum, reset);
    }
    printf("\n");
}
