
int get_piece_moves(Piece piecemap[64], int square, Player turn, Move **moves) 
{
    if (piecemap[square] == NO_PIECE) return 0;
    if (turn == PLAYER_WHITE && is_black[piecemap[square]]) return 0;
    if (turn == PLAYER_BLACK && is_white[piecemap[square]]) return 0;
    if (PIECE_MAP[piecemap[square]] == 'P') return pawn_moves(piecemap, square, turn, moves);
    if (PIECE_MAP[piecemap[square]] == 'N') return knight_moves(piecemap, square, moves);
    if (PIECE_MAP[piecemap[square]] == 'B') return bishop_moves(piecemap, square, moves);
    if (PIECE_MAP[piecemap[square]] == 'R') return rook_moves(piecemap, square, moves);
    if (PIECE_MAP[piecemap[square]] == 'Q') return queen_moves(piecemap, square, moves);
    if (PIECE_MAP[piecemap[square]] == 'K') return king_moves(piecemap, square, moves);
    return 0;
}

int square_is_attacked(Piece piecemap[64], int square, Player attacker) {
    int i, j, move_count = 0;
    Move **move_holder = malloc(sizeof(Move*)*20);
    for (i = 0; i < 64; i++) {
        move_count = get_piece_moves(piecemap, i, attacker, move_holder);
        for (j = 0; j < move_count; j++) {
            if ((*(move_holder + j))->main->to == square) {
                free(move_holder);
                return 1;
            }
        }
    }
    free(move_holder);
    return 0;
}
        

MoveSet trim_invalid_moves(Board *b, MoveSet m)
{
    int i, k = 0;
    b->turn = !b->turn; 
    for (i = 0; i < m.count; i++) {
        applyMove(b, *(m.moves+i));
        if (locate_king(b->piecemap, b->turn) != -1 
                && locate_king(b->piecemap, !b->turn) != -1
                && !square_is_attacked(b->piecemap, locate_king(b->piecemap, !(b->turn)), b->turn))
        *(m.moves + k++) = *(m.moves + i);
        reverseMove(b, *(m.moves+i));
    }
    m.count = k;
    return m;
}
