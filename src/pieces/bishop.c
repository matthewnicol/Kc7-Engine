int bishop_moves(Piece piecemap[64], int square, Move **moves) 
{
    return lateral_mover(piecemap, square, 0, 1, moves);
}
