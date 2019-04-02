/* Functions for getting information about positions */

MoveSet valid_moves(Board *b) /* List all valid moves on a board */
{
    int i;
    Move **moves = malloc(sizeof(Move*)*60);
    MoveSet mset = {0, moves};
    Board bscratch;
    copy_board(b, &bscratch);
    for (i = 0; i < 64; i++) {
        mset.count += get_piece_moves(b->piecemap, i, b->turn, (mset.moves + mset.count));
    }

    mset = trim_invalid_moves(&bscratch, mset);
    return mset;
} 

int is_checkmate(Board *b, MoveSet mset)
{
    // No valid moves left
    return mset.count == 0 
        // King is attacked
        && square_is_attacked(b->piecemap, locate_king(b->piecemap, b->turn), !b->turn);
}

int is_stalemate(Board *b, MoveSet mset)
{
    // No valid moves left
    return mset.count == 0
        // But king is not attacked
        && !is_checkmate(b, mset);
}

int handle_position(PositionStrategy strat, Board *b) 
{
    if (strat == MAKE_RANDOM_MOVE) {
        MoveSet mset = valid_moves(b);
        int i;
        for (i = 0; i < mset.count; i++) {
            char *moverepr = move_to_algebraic(*(mset.moves + i));
            printf("%s\t", moverepr);
            if (i % 5 == 0 && i > 0)  printf("\n");
        }
        printf("\n\n");
        if (*(mset.moves+0) != NULL) {
            applyMove(b, *(mset.moves+rand()%mset.count));
            b->turn = !b->turn;
        } else {
            if (is_checkmate(b, mset)) {
                printf("Checkmate!\n");
            } else if (is_stalemate(b, mset)) {
                printf("Stalemate!\n");
            }
        }
    }
    return 0;
}


int castling_available(PieceMap p, int kingside, Turn t) /* Can the player castle in the current position */
{
    int ln = t? 0 : 55;
    Piece r = t? BLACK_CASTLING_ROOK : WHITE_CASTLING_ROOK;
    Piece k = t? BLACK_CASTLING_KING : WHITE_CASTLING_KING;
    return (p[ln+4] == k) 
        && (  (kingside && p[ln+7] == r && !p[ln+6] && !p[ln+5]) 
           || (!kingside && p[ln] == r && !p[ln+1] && !p[ln+2] && !p[ln+3]));
}
