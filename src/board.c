Board new_board() {
    int i;
    Board b;
    b.turn = PLAYER_WHITE;
    b.count = 0;
    for (i = 0; i < 64; i++) b.squares[i] = NO_PIECE;
    return b;
}

void FEN(char *fen, Board *b) 
{
    int i, sq = 0;

    // Where the pieces are
    for (i = 0; fen[i] != '0'; i++) {
        if (fen[i] == '/') continue;
        if (fen[i] > '0' && fen[i] < '9') {
            int k = (int)(fen[i] - '0');
            while (k-- > 0) {
                b->squares[sq++] = NO_PIECE;
            }
        } else {
            b->squares[sq++] = chr_to_piece(fen[i]);
        }
    }
    
    // Whose turn it is in the current position
    b->turn = (fen[++i] == 'w' ? PLAYER_WHITE : PLAYER_BLACK);

    // Kingside and Queenside castling rights
    for (i+=2; fen[i] != ' '; i++) {
        if (fen[i] == 'K') {
            b->squares[4] = BLACK_CASTLING_KING;
            b->squares[7] = BLACK_CASTLING_ROOK;
        }
        if (fen[i] == 'Q') {
            b->squares[4] = BLACK_CASTLING_KING;
            b->squares[0] = BLACK_CASTLING_ROOK;
        }
        if (fen[i] == 'k') {
            b->squares[4] = BLACK_CASTLING_KING;
            b->squares[60] = BLACK_CASTLING_ROOK;
        }
        if (fen[i] == 'q') {
            b->squares[60] = BLACK_CASTLING_KING;
            b->squares[56] = BLACK_CASTLING_ROOK;
        }
    }
    
    // En Passant Square
    if (fen[(i+=2)] != '-') {
        b->squares[algebraic_to_sq(fen[i], fen[i+1])] = 
            b->squares[algebraic_to_sq(fen[i], fen[i+1])] == WHITE_PAWN ? 
                WHITE_EP_PAWN : BLACK_EP_PAWN;
    }
}

void standard_position(Board *b)
{ 
    FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", b);
}

#define RP(X)   (COLOUR_PIECE_MAP[p[X]])

void printBoard(Piece p[])
{
    int j = 0, i = 0;
    for (i = 0; i < 8; i++) {
        printf("\t");
        for (j = 0; j < 8; j++) printf("%c ", RP(i*8+j));
        printf("\n");
    }
}
