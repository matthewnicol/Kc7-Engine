static int knight_differentials[][2] = {
    {1, 2}, {1, -2}, {-1, -2}, {-1, 2}, {2, 1}, {2, -1}, {-2, -1}, {-2, 1}
};

static char invalid_knight_files[][2] = {{'a', 'g'}, {'a', 'h'}, {'b', 'h'}, {'h', 'a'}, {'h', 'b'}, {'g', 'a'},
    {'\0', '\0'}};

int knight_moves(Piece piecemap[], int square, Move **moves) 
{
    int i, k = 0;
    for (i = 0; i < 8; i++) {
        int sq2 = square + (knight_differentials[i][0]*8) + knight_differentials[i][1];

        // Make sure square is on the board & we're not already in it
        if (!VALID(sq2) 
                || same_team(piecemap, square, sq2) 
                || tuple_matches(FILE_MAP[square], FILE_MAP[sq2], invalid_knight_files)) continue;

        moves[k++] = makeSimpleMove(square, sq2, piecemap[square], piecemap[sq2]); 
    }
    return k;
}
