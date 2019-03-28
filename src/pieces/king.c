static int king_differentials[][2] = {
    {1, 1}, {1, -1}, {-1, -1}, {-1, 1}, {0, 1}, {0, -1}, {1, 0}, {-1, 0}
};

static char invalid_king_files[][2] = {{'a', 'h'}, {'h', 'a'}, {'\0', '\0'}};

int king_moves(Piece piecemap[64], int square, Player turn, Move **moves) 
{
    int i, sq2, k = 0;
    Piece moved = turn ? BLACK_MOVED_KING : WHITE_MOVED_KING;

    for (i = 0; i < 8; i++) {
        sq2 = square + king_differentials[i][0]*8 + knight_differentials[i][1];
        if (!VALID(sq2)
                || tuple_matches(FILE_MAP[square], FILE_MAP[sq2], invalid_king_files)
                || same_team(piecemap, square, sq2)) 
            continue;

        // Normal king movement
        moves[k++] = makeSimpleMove(square, sq2, moved, piecemap[sq2]);
    }

    // Castling Moves - Black
    if (piecemap[4] == BLACK_STILL_KING && square == 4)
        if (turn && !piecemap[5] && !piecemap[6] && piecemap[7] == BLACK_STILL_ROOK)
            moves[k++] = makeMove(
                makePieceMovement(square, 6, moved, NO_PIECE),
                makePieceMovement(7, 5, BLACK_MOVED_ROOK, NO_PIECE));
        else if (turn && !piecemap[1] && !piecemap[2] && !piecemap[3] && piecemap[7] == BLACK_STILL_ROOK)
            moves[k++] = makeMove(
                makePieceMovement(square, 2, moved, NO_PIECE),
                makePieceMovement(0, 3, BLACK_MOVED_ROOK, NO_PIECE));

    // Castling Moves - White
    if (piecemap[60] == BLACK_STILL_KING && square == 60)
        if (turn && !piecemap[61] && !piecemap[62] && piecemap[63] == WHITE_STILL_ROOK)
            moves[k++] = makeMove(
                makePieceMovement(square, 62, moved, NO_PIECE),
                makePieceMovement(63, 61, WHITE_MOVED_ROOK, NO_PIECE));
        else if (turn && !piecemap[59] && !piecemap[58] && !piecemap[57] && piecemap[56] == WHITE_STILL_ROOK)
            moves[k++] = makeMove(
                makePieceMovement(square, 58, moved, NO_PIECE),
                makePieceMovement(56, 59, WHITE_MOVED_ROOK, NO_PIECE));
    return k;
}
