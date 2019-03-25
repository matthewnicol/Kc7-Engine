PieceMovement* blankPieceMovement ()
{
    return NULL;
}

PieceMovement* makePieceMovement (int from, int to, Piece on_from, Piece on_to)
{
    PieceMovement *p = malloc(sizeof(PieceMovement));
    (*p).from = from;
    (*p).to = to;
    (*p).on_from = on_from;
    (*p).on_to = on_to;
    return p;
}

Move *makeMove (PieceMovement *main, PieceMovement *alt)
{
    Move *m = malloc(sizeof(Move));
    m->main = main;
    m->alt = alt;
    return m;
}

void validMoves(Board *b) /* List all valid moves on a current board */
{
    int move_count = 0;
    int i;
    Move **moves = malloc(sizeof(Move*)*30);
    for (i = 0; i < 64; i++) {
        if ((is_white[b->piecemap[i]] && b->turn == PLAYER_WHITE) || (is_black[b->piecemap[i]] && b->turn == PLAYER_BLACK)) {
            if      (PIECE_MAP[b->piecemap[i]] == "P") move_count += pawn_moves  (b, moves+move_count, i);
            else if (PIECE_MAP[b->piecemap[i]] == "N") move_count += knight_moves(b, moves+move_count, i);
            else if (PIECE_MAP[b->piecemap[i]] == "B") move_count += bishop_moves(b, moves+move_count, i);
            else if (PIECE_MAP[b->piecemap[i]] == "R") move_count += rook_moves  (b, moves+move_count, i);
            else if (PIECE_MAP[b->piecemap[i]] == "Q") move_count += queen_moves (b, moves+move_count, i);
            else if (PIECE_MAP[b->piecemap[i]] == "K") move_count += king_moves  (b, moves+move_count, i);
        }
    }
    printf("\nTotal Moves: %i\n", move_count);

    for (i = 0; i < move_count; i++) {
        if (moves[i] != NULL)
        reprMove(*(moves+i));
    }
        

} 

/* Does a board have the same side's piece in sq1 and sq2 */
int same_team(Board *b, int sq1, int sq2) {
    return (is_black[b->piecemap[sq1]] && is_black[b->piecemap[sq2]]) ||
        (is_white[b->piecemap[sq1]] && is_white[b->piecemap[sq2]]);
}
