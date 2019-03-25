int queen_moves(Board *b, Move **moves, int square) 
{
    int i, j;
    int k=0;
    int blockaded[] = {0, 0, 0, 0, 0, 0, 0, 0};
    char *files[] = {FILE_MAP[square], FILE_MAP[square], FILE_MAP[square], FILE_MAP[square],FILE_MAP[square],FILE_MAP[square],FILE_MAP[square],FILE_MAP[square]};
    for (i = 1; i < 9; i++) {
        int differentials[] = {
            square + (8*i) - i, square + (8*i) + i, square - (8*i) - i, square - (8*i) + i,
            square + (8*i), square + (8*i), square - (8*i), square - (8*i)
        };
        for (j = 0; j < 8; j++) {
            if (blockaded[j] || !VALID(differentials[j])) continue;  

            // Don't jump off the board
            if (files[j] == "h" && FILE_MAP[differentials[j]] == "a") { blockaded[j] = 1; continue; }
            if (files[j] == "a" && FILE_MAP[differentials[j]] == "h") { blockaded[j] = 1; continue; }
            files[j] = FILE_MAP[differentials[j]];

            blockaded[j] = b->piecemap[differentials[j]] != NO_PIECE;
            if (blockaded[j]) {
                if (is_white[b->piecemap[differentials[j]]] && is_white[b->piecemap[square]]) continue;
                if (is_black[b->piecemap[differentials[j]]] && is_black[b->piecemap[square]]) continue;
            }
            moves[k++] = makeMove(
                    makePieceMovement(square, differentials[j], b->piecemap[square], b->piecemap[differentials[j]]),
                    blankPieceMovement());
        }
    }
    return k;
}
