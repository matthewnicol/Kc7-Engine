int locate_king(Piece piecemap[64], Player colour)
{
    int i;
    for (i = 0; COLOUR_PIECE_MAP[piecemap[i++]] != (colour == PLAYER_WHITE ? 'k' : 'K'););
    return i;
}
