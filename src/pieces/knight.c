static int knight_differentials[][2] = {
    {1, 2}, {1, -2}, {-1, -2}, {-1, 2}, {2, 1}, {2, -1}, {-2, -1}, {-2, 1}
};

int knight_moves(Piece piecemap[64], int square, Player turn, Move **moves) 
{
    int i, k = 0;
    for (i = 0; i < 8; i++) {
        int sq2 = square + (knight_differentials[i][0]*8) + knight_differentials[i][1];

        // Make sure square is on the board & we're not already in it
        if (!VALID(sq2) || same_team(piecemap, square, sq2)) continue;

        // Don't jump off the board
        if (FILE_MAP[square] == "a" && (FILE_MAP[sq2] == "g" || FILE_MAP[sq2] == "h")) continue;
        if (FILE_MAP[square] == "b" && FILE_MAP[sq2] == "h") continue;
        if (FILE_MAP[square] == "h" && (FILE_MAP[sq2] == "a" || FILE_MAP[sq2] == "b")) continue;
        if (FILE_MAP[square] == "g" && FILE_MAP[sq2] == "a") continue;

        // Add move if all checks pass
        moves[k++] = makeMove(
            makePieceMovement(square, sq2, piecemap[square], piecemap[sq2]), 
            blankPieceMovement()
        );
    }
    return k;
}
