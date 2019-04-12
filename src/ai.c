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

float minmax(Board *b, int depth)
{
    MoveSet *m = all_legal_moves(b->squares, b->turn);
    float result = 1000.0, tmp = 0.0;

    if (depth == 0) {
        result = evaluate(b, m);
        free(m);
        return result;
    } else {
        b->turn = !b->turn;
        int i;
        for (i = 0; i < m->count; i++) {
            apply_move(b->squares, &m->moves[i]);
            if (result == 1000) {
                result = minmax(b, depth-1);
            } else {
                tmp = minmax(b, depth-1);
                if ((b->turn && tmp < result) || (b->turn && tmp > result)) {
                    result = tmp;
                }
            }
            reverse_move(b->squares, &m->moves[i]);
        }
    }
        
    free(m);
    b->turn = !b->turn;
    return result;
}
