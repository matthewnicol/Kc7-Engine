/* Get standard board configuration */
void standard (Board *b) 
{
    int i;
    for (i = 0; i < 8; i++) {
        b->piecemap[i+8] = BLACK_PAWN;
        b->piecemap[63-8-i] = WHITE_PAWN;
    }
    b->piecemap[0] = b->piecemap[7] = BLACK_STILL_ROOK;
    b->piecemap[1] = b->piecemap[6] = BLACK_KNIGHT;
    b->piecemap[2] = b->piecemap[5] = BLACK_BISHOP;
    b->piecemap[63-0] = b->piecemap[63-7] = WHITE_STILL_ROOK;
    b->piecemap[63-1] = b->piecemap[63-6] = WHITE_KNIGHT;
    b->piecemap[63-2] = b->piecemap[63-5] = WHITE_BISHOP;
    b->piecemap[4] = BLACK_QUEEN;
    b->piecemap[59] = WHITE_QUEEN;
    b->piecemap[3] = BLACK_STILL_KING;
    b->piecemap[60] = WHITE_STILL_KING;
}

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

Board *copy_board(Board *b)
{
    Board *b2 = malloc(sizeof(Board));
    int i;
    for (i = 0; i < 64; i++) b2->piecemap[i] = b->piecemap[i];
    b2->turn = b->turn;
    return b2;
}
