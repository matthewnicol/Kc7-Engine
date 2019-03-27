static int king_differentials[][2] = {
    {1, 1}, {1, -1}, {-1, -1}, {-1, 1}, {0, 1}, {0, -1}, {1, 0}, {-1, 0}
};

int king_moves(Piece piecemap[64], int square, Player turn, Move **moves) 
{
    int i, sq2;
    int k = 0;
    char *file = FILE_MAP[square];
    Piece moved = turn ? BLACK_MOVED_KING : WHITE_MOVED_KING;
    for (i = 0; i < 8; i++) {
        int sq2 = square + king_differentials[i][0]*8 + knight_differentials[i][1];
        if (!VALID(sq2)) continue;
        
        // Don't jump off board;
        if (file == "a" && FILE_MAP[sq2] == "h") continue;
        if (file == "h" && FILE_MAP[sq2] == "a") continue;
        file = FILE_MAP[sq2];

        if (is_black[piecemap[square]] && is_black[piecemap[sq2]]) continue;
        if (is_white[piecemap[square]] && is_white[piecemap[sq2]]) continue;

        // Normal king movement
        moves[k++] = makeMove(
                makePieceMovement(square, sq2, moved, piecemap[sq2]),
                blankPieceMovement());

        // Kingside Castle - Black
        if (turn && !piecemap[5] && !piecemap[6] && piecemap[7] == BLACK_STILL_ROOK)
            moves[k++] = makeMove(
                    makePieceMovement(square, 6, moved, NO_PIECE),
                    makePieceMovement(7, 5, BLACK_MOVED_ROOK, NO_PIECE));


    }
    return k;
}
