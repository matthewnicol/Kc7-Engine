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
    b->captures = malloc(sizeof(Piece)*100);
    if (b->captures == NULL) {
        free(b->squares);
        free(b->moves);
        free(b);
    }
    b->turn = PLAYER_WHITE;
    b->move_number = 0;
    b->count = 0;
    for (int i = 0; i < 64; i++) b->squares[i]= NO_PIECE;
    return b;
}

long *sq_hashes;

int get_piece_index(Piece p) {
    int piece_index;
    switch (p) {
        case BLACK_PAWN : piece_index = 0; break;
        case BLACK_EP_PAWN : piece_index = 1; break;
        case BLACK_CASTLING_ROOK : piece_index = 2; break;
        case BLACK_ROOK : piece_index = 3; break;
        case BLACK_CASTLING_KING : piece_index = 4; break;
        case BLACK_KING : piece_index = 5; break;
        case BLACK_KNIGHT : piece_index = 6; break;
        case BLACK_BISHOP : piece_index = 7; break;
        case BLACK_QUEEN : piece_index = 8; break;
        case WHITE_PAWN : piece_index = 9; break;
        case WHITE_EP_PAWN : piece_index = 10; break;
        case WHITE_CASTLING_ROOK : piece_index = 11; break;
        case WHITE_ROOK : piece_index = 12; break;
        case WHITE_CASTLING_KING : piece_index = 13; break;
        case WHITE_KING : piece_index = 14; break;
        case WHITE_KNIGHT : piece_index = 15; break;
        case WHITE_BISHOP : piece_index = 16; break;
        case WHITE_QUEEN : piece_index = 17; break;
        default: break; 
    } 
    return piece_index;

}

long position_hash(Board *b) 
{
    if (sq_hashes == NULL) {
        sq_hashes = malloc(sizeof(long)*18*64);
        for (int i = 0; i < 18*64; i++) {
            sq_hashes[i] = rand();
        }
    }
    b->hash = rand();
    for (int i = 0; i < 64; i++) {
        if (b->squares[i] != ' ') {
            int piece_index = get_piece_index(b->squares[i]);
            b->hash = b->hash ^ sq_hashes[piece_index * i];
        }
    }

}

void reverse_move_hash(Board *b, Move *m)
{
    // Remove piece at 'to' square
    b->hash = b->hash ^ sq_hashes[m->to * get_piece_index(b->squares[m->from])];

    // Add captured piece from 'to' square if existing
    if (b->captures[b->move_number] != ' ') {
        b->hash = b->hash ^ sq_hashes[m->to * get_piece_index(b->captures[b->move_number])];
    }

    // Add piece that moved back on original 'from' square
    b->hash = b->hash ^ sq_hashes[m->from * get_piece_index(b->squares[m->to])];
}

void apply_move_hash(Board *b, Move *m)
{
    // Remove piece at 'from' square
    b->hash = b->hash ^ sq_hashes[m->from * get_piece_index(b->squares[m->to])];

    // Remove existing piece at 'to' square
    if (b->captures[b->move_number-1] != ' ') {
        b->hash = b->hash ^ sq_hashes[m->to * get_piece_index(b->captures[b->move_number-1])];
    }

    // Add 'from' piece to 'to' square
    b->hash = b->hash ^ sq_hashes[m->to * get_piece_index(b->squares[m->to])];


}

int is_checkmate(Board *b, MoveSet *m)
{
   return m->count == 0 && square_is_attacked(b, m->king_pos);
}

int is_stalemate(Board *b, MoveSet *m)
{
   return m->count == 0 && !square_is_attacked(b, m->king_pos);
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
            }
        } else {
            b->squares[sq++] = fen[i];
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
    position_hash(b);
}

void standard_position(Board *b)
{ 
    FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", b);
}

void apply_move(Board *b, Move *m)
{

    b->captures[b->move_number++] = b->squares[m->to];

    b->squares[m->to] = b->squares[m->from];
    b->squares[m->from] = NO_PIECE;

    apply_move_hash(b, m);

    // Reset previous potential EP capture squares
    if (ISWHITE(b->squares[m->to]))
        for (int i = 24; i < 32; i++)
            if (b->squares[i] == BLACK_EP_PAWN) b->squares[i] = BLACK_PAWN;

    // Reset previous potential EP capture squares
    if (ISBLACK(b->squares[m->to]))
        for (int i = 32; i < 40; i++)
            if (b->squares[i] == WHITE_EP_PAWN) b->squares[i] = WHITE_PAWN;

    // Mark potential EP capture squares
    if (m->to == m->from + 16 && b->squares[m->to] == BLACK_PAWN) b->squares[m->to] = BLACK_EP_PAWN;
    if (m->to == m->from - 16 && b->squares[m->to] == WHITE_PAWN) b->squares[m->to] = WHITE_EP_PAWN;

    if (m->side_effect == KS_CASTLE) {
        b->squares[m->from+1] = b->squares[m->to] == WHITE_CASTLING_KING ? WHITE_ROOK : BLACK_ROOK;
        b->squares[m->to+1] = NO_PIECE;
    } else if (m->side_effect == QS_CASTLE) {
        b->squares[(m->from)-2] = b->squares[m->to] == WHITE_CASTLING_KING ? WHITE_ROOK : BLACK_ROOK;
        b->squares[(m->to)+4] = NO_PIECE;
    } else if (m->side_effect == EP_CAPTURE) {
        b->squares[m->to + (8 * (ISWHITE(b->squares[m->to]) ? 1 : -1))] = NO_PIECE;
    } else if (m->side_effect == PROMOTION) {
        // this side effect is needed for reverse_move
    }
}

void reverse_move(Board *b, Move *m)
{
    b->squares[m->from] = b->squares[m->to];
    b->squares[m->to] = b->captures[--(b->move_number)];

    // Reset previous potential EP capture squares
    if (ISWHITE(b->squares[m->from]))
        for (int i = 48; i < 56; i++)
            if (b->squares[i] == WHITE_EP_PAWN) b->squares[i] = WHITE_PAWN;

    // Reset previous potential EP capture squares
    if (ISBLACK(b->squares[m->from]))
        for (int i = 8; i < 16; i++)
            if (b->squares[i] == BLACK_EP_PAWN) b->squares[i] = BLACK_PAWN;
   
    reverse_move_hash(b, m);

    if (m->side_effect == KS_CASTLE) {
        b->squares[m->from] = b->squares[m->to] == WHITE_KING ? WHITE_CASTLING_KING : BLACK_CASTLING_KING;
        b->squares[m->to+1] = b->squares[m->from] == WHITE_CASTLING_KING? WHITE_CASTLING_ROOK : BLACK_CASTLING_ROOK;
        b->squares[m->to-1] = NO_PIECE;
    } else if (m->side_effect == QS_CASTLE) {
        b->squares[m->from] = b->squares[m->to] == WHITE_KING ? WHITE_CASTLING_KING : BLACK_CASTLING_KING;
        b->squares[m->from-2] = b->squares[m->to] == WHITE_CASTLING_KING ? WHITE_CASTLING_ROOK : BLACK_CASTLING_ROOK;
        b->squares[m->to+1] = NO_PIECE;
    } else if (m->side_effect == EP_CAPTURE) {
        b->squares[m->to + (8 * (ISWHITE(b->squares[m->to]) ? 1 : -1))] = b->squares[m->from] == WHITE_PAWN ? BLACK_EP_PAWN : WHITE_EP_PAWN;
    } else if (m->side_effect == PROMOTION) {
        // this side effect is needed for reverse_move
    }
}

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
            char pc = p[i*8+j];
            printf("%s%4c%s %s|%s", 
                ISBLACK(p[i*8+j])? black : white,
                //PIECEREP(p[i*8+j]),
                ISPIECE(pc) ? pc : ' ',
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
