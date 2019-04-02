
char reprPiece(int i) {
    return COLOUR_PIECE_MAP[i];
}


void printBoard(Board *b)
{
    int i = 0;
    for (i = 0; i < 8; i++) 
        printf("\t%c %c %c %c %c %c %c %c\n",
            reprPiece(b->piecemap[i*8+0]),
            reprPiece(b->piecemap[i*8+1]),
            reprPiece(b->piecemap[i*8+2]),
            reprPiece(b->piecemap[i*8+3]),
            reprPiece(b->piecemap[i*8+4]),
            reprPiece(b->piecemap[i*8+5]),
            reprPiece(b->piecemap[i*8+6]),
            reprPiece(b->piecemap[i*8+7]));
}
