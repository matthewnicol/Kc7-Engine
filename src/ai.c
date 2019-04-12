void make_random_move(Board *b, MoveSet *m)
{
    time_t t;
    srand((unsigned)time(&t));
    if (m->count > 0) {
        int randmove = rand()%m->count;
        printf("Picked random move: %i", randmove);
        apply_move(b->squares, m->moves+randmove);
    }
}

float evaluate(Board *b, MoveSet *m) 
{
    if (is_checkmate(b->squares, m)) {
        return b->turn ? -999 : 999;
    }
    int i, piece_scores = 0;
    int bishops[] = {0, 0};
    // Close game
    for (i = 0; i < 64; i++) {
        piece_scores += PIECE_VALUE_MAP[b->squares[i]];
        if (is_bishop[b->squares[i]]) bishops[is_black[b->squares[i]]]++;
    }
    return piece_scores + (bishops[0] == 2 ? .5 : 0.0) + (bishops[1] == 2 ? -.5 : 0.0);
}
