int bishop_moves(Board *b, Move **moves, int square) 
{
    int i, j, k;
    k=0;
    int blockaded[] = {0, 0, 0, 0};
    char *files[] = {FILE_MAP[square], FILE_MAP[square], FILE_MAP[square], FILE_MAP[square]};
    for (i = 1; i < 9; i++) {
        int differentials[] = {
            square + (8*i) - i,
            square + (8*i) + i,
            square - (8*i) - i,
            square - (8*i) + i
        };
        if (blockaded[0] == 1 && blockaded[1] == 1 && blockaded[2] == 1 && blockaded[3] == 1) return k;
        for (j = 0; j < 4; j++) {
            
            if (blockaded[j] || !VALID(differentials[j])) continue;
            
            // Don't jump off board
            if (files[j] == "h" && FILE_MAP[differentials[j]] == "a") { blockaded[j] = 1; continue; }
            if (files[j] == "a" && FILE_MAP[differentials[j]] == "h") { blockaded[j] = 1; continue; }
            files[j] = FILE_MAP[differentials[j]];

            
            blockaded[j] = b->piecemap[ differentials[j] ] != NO_PIECE;
            if (blockaded[j]) {
                if (same_team(b, square, differentials[j])) continue;
            }
                moves[k++] = makeMove(
                        makePieceMovement(square, differentials[j], b->piecemap[square], b->piecemap[differentials[j]]),
                        blankPieceMovement());

        }
    }
    return k;
}
