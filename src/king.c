static int king_differentials[][2] = {
    {1, 1}, {1, -1}, {-1, -1}, {-1, 1}, {0, 1}, {0, -1}, {1, 0}, {-1, 0}
};

int king_moves(Board *b, Move **moves, int square) 
{
    int i, sq2;
    int k = 0;
    char *file = FILE_MAP[square];
    for (i = 0; i < 8; i++) {
        int sq2 = square + king_differentials[i][0]*8 + knight_differentials[i][1];
        if (!VALID(sq2)) continue;
        
        // Don't jump off board;
        if (file == "a" && FILE_MAP[sq2] == "h") continue;
        if (file == "h" && FILE_MAP[sq2] == "a") continue;
        file = FILE_MAP[sq2];

        if (is_black[b->piecemap[square]] && is_black[b->piecemap[sq2]]) continue;
        if (is_white[b->piecemap[square]] && is_white[b->piecemap[sq2]]) continue;
        moves[k++] = makeMove(
                makePieceMovement(square, sq2, b->piecemap[square], b->piecemap[sq2]),
                blankPieceMovement());
    }
    return k;
}
