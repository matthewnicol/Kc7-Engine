int queen_moves(Piece piecemap[64], int square, Move **moves) 
{
    return lateral_mover(piecemap, square, 1, 1, moves);
}
