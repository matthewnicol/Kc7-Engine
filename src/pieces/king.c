static int king_differentials[][2] = {
    {1, 1}, {1, -1}, {-1, -1}, {-1, 1}, {0, 1}, {0, -1}, {1, 0}, {-1, 0}
};

static char invalid_king_files[][2] = {{'a', 'h'}, {'h', 'a'}, {'\0', '\0'}};

int king_moves(Piece piecemap[64], int square, Move **moves) 
{
    int i, k = 0;
    Turn turn = is_black[piecemap[square]] ? PLAYER_BLACK : PLAYER_WHITE;
    Piece moved = is_black[piecemap[square]] ? BLACK_KING : WHITE_KING;

    for (i = 0; i < 8; i++) {
        int sq2;
        sq2 = square + king_differentials[i][0]*8 + knight_differentials[i][1];
        if (!VALID(sq2)
                || tuple_matches(FILE_MAP[square], FILE_MAP[sq2], invalid_king_files)
                || same_team(piecemap, square, sq2)) 
            continue;

        // Normal king movement
        moves[k++] = makeSimpleMove(square, sq2, moved, piecemap[sq2]);
    }

    // Castling Moves - Black
    if (castling_available(piecemap, 1, turn))
        moves[k++] = makeMove(
                makePieceMovement(square, 6, moved, NO_PIECE),
                makePieceMovement(7, 5, turn ? BLACK_ROOK : WHITE_ROOK, NO_PIECE));
    if (castling_available(piecemap, 0, turn))
            moves[k++] = makeMove(
                makePieceMovement(square, 2, moved, NO_PIECE),
                makePieceMovement(0, 3, turn ? BLACK_ROOK : WHITE_ROOK, NO_PIECE));
    return k;
}
