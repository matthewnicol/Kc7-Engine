int lateral_mover(Piece piecemap[64], int square, Player turn, int updown, int diagonal, Move **moves) 
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
            if (!updown && j > 3) continue;
            if (!diagonal && j < 4) continue;
            if (blockaded[j] || !VALID(differentials[j])) continue;  

            // Don't jump off the board
            if (files[j] == "h" && FILE_MAP[differentials[j]] == "a") { blockaded[j] = 1; continue; }
            if (files[j] == "a" && FILE_MAP[differentials[j]] == "h") { blockaded[j] = 1; continue; }
            files[j] = FILE_MAP[differentials[j]];

            blockaded[j] = piecemap[differentials[j]] != NO_PIECE;
            if (blockaded[j]) {
                if (is_white[piecemap[differentials[j]]] && is_white[piecemap[square]]) continue;
                if (is_black[piecemap[differentials[j]]] && is_black[piecemap[square]]) continue;
            }
            moves[k++] = makeMove(
                    makePieceMovement(square, differentials[j], piecemap[square], piecemap[differentials[j]]),
                    blankPieceMovement());
        }
    }
    return k;
}
