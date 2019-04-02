int bishop_moves(Piece piecemap[], int square, Move **moves) 
{
    return lateral_mover(piecemap, square, 0, 1, moves);
}
