int locate_king(Piece piecemap[64], Player colour)
{
    int i;
    for (i = 0; i < 64; i++) {
        assert(piecemap[i] < 22);
        if (is_white[piecemap[i]] && is_king[piecemap[i]] && colour == PLAYER_WHITE) return i;
        if (is_black[piecemap[i]] && is_king[piecemap[i]] && colour == PLAYER_BLACK) return i;
    }

    return -1;
}
