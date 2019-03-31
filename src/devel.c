
char reprPiece(int i) {
    return COLOUR_PIECE_MAP[i];
}

void reprMove(Move *m) {
    printf("\t");
   
    if (is_king[m->main->on_from]) {
        if (m->main->from == 60 && m->main->to == 62) { printf("0-0"); return; }
        if (m->main->from == 60 && m->main->to == 58) { printf("0-0-0"); return; }
        if (m->main->from == 4 && m->main->to == 6) { printf("0-0"); return; }
        if (m->main->from == 4 && m->main->to == 2) { printf("0-0-0"); return; }
    }

    if (is_pawn[m->main->on_from]) {
        if (FILE_MAP[m->main->to] != FILE_MAP[m->main->from]) {
            printf("%cx%c%i", FILE_MAP[m->main->from], FILE_MAP[m->main->to], RANK_MAP[m->main->to]); 
        } else printf("%c%i", FILE_MAP[m->main->on_from], RANK_MAP[m->main->to]);
        return;
    }
    printf("%c%s%c%i", 
            PIECE_MAP[m->main->on_from], 
            PIECE_MAP[m->main->on_to]? "x" : "",
            FILE_MAP[m->main->to],
            RANK_MAP[m->main->to]
            );
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
