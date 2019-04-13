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

double evaluate(Board *b, MoveSet *m) 
{
    if (is_checkmate(b->squares, m)) {
        return b->turn ? -999.0 : 999.0;
    } else if (is_stalemate(b->squares, m)) {
            return 0.0;
    }
    int i; 
    float piece_scores = 0;
    int bishops[] = {0, 0};
    // Close game
    for (i = 0; i < 64; i++) {
        piece_scores += PIECE_VALUE_MAP[b->squares[i]];
        if (is_bishop[b->squares[i]]) bishops[is_black[b->squares[i]]]++;
    }
    return piece_scores + (bishops[0] == 2 ? .5 : 0.0) + (bishops[1] == 2 ? -.5 : 0.0);
}

double minmax(Board *b, int depth, Move *outmove)
{
    MoveSet *m = all_legal_moves(b->squares, b->turn);
    
    if (depth == 0) {
        double result = evaluate(b, m);
        free(m->moves);
        free(m);
        return result;
    } else {
        double tmp, best;
        b->turn = !b->turn;
        apply_move(b->squares, &m->moves[0]);
        printf("A NEW CONTENDER");
        printMove(9999, &m->moves[0]);
        best = minmax(b, depth-1, outmove);
        (*outmove).from = m->moves[0].from;
        (*outmove).to = m->moves[0].to;
        (*outmove).on_from = m->moves[0].on_from;
        (*outmove).on_to = m->moves[0].on_to;
        (*outmove).side_effect = m->moves[0].side_effect;
        reverse_move(b->squares, &m->moves[0]);
        int i;
        for (i = 1; i < m->count; i++) {
            apply_move(b->squares, &m->moves[i]);
            tmp = minmax(b, depth-1, outmove);
            if ((b->turn && tmp < best) || (!b->turn && tmp > best)) {
                best = tmp;
                printf("A NEW CONTENDER");
                printMove(9999, &m->moves[i]);
                (*outmove).from = m->moves[i].from;
                (*outmove).to = m->moves[i].to;
                (*outmove).on_from = m->moves[i].on_from;
                (*outmove).on_to = m->moves[i].on_to;
                (*outmove).side_effect = m->moves[0].side_effect;
            }
            reverse_move(b->squares, &m->moves[i]);
        }
        free(m->moves); 
        free(m);
        b->turn = !b->turn;
        return best;
    }
}

