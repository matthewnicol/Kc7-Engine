int queen_moves(Piece piecemap[64], int square, Player turn, Move **moves) 
{
    return lateral_mover(piecemap, square, turn, 1, 1, moves);
}
