
#define RP(X)   (COLOUR_PIECE_MAP[b->piecemap[X]])

void printBoard(Board *b)
{
    int j = 0, i = 0;
    for (i = 0; i < 8; i++) {
        printf("\t");
        for (j = 0; j < 8; j++) printf("%c ", RP(i*8+j));
        printf("\n");
    }
}
