int rook_moves(Piece piecemap[], int square, Move **moves) 
{
    return lateral_mover(piecemap, square, 1, 0, moves);
}
