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

#define WHITEBLACK_VAL(T, A, B) (T == PLAYER_WHITE ? A : B)
#define DEFAULT_EVAL(T) WHITEBLACK_VAL(T, -1000.00, 1000.00)

double evaluate(Piece *sq, MoveSet *m, Player p) 
{
    if (is_checkmate(sq, m)) {
        return WHITEBLACK_VAL(p, -1000.00, 1000.00);
    } else if (is_stalemate(sq, m)) {
        return 0.0;
    }
    int i; 
    float piece_scores = 0;
    int bishops[] = {0, 0};
    // Close game
    for (i = 0; i < 64; i++) {
        piece_scores += PIECE_VALUE_MAP[sq[i]];
        if (is_bishop[sq[i]]) bishops[is_black[sq[i]]]++;
    }
    return piece_scores + (bishops[0] == 2 ? .5 : 0.0) + (bishops[1] == 2 ? -.5 : 0.0);
}

#define SEARCHDEPTH 2
#define BETTER_EVAL(T, A, B) ((T == PLAYER_WHITE && A > B) || (T == PLAYER_BLACK && A < B))

Move minimax_choice(Piece *sq, MoveSet *m, Player p)
{
    Move choice;
    double 
        tmp_evaluation,
        best_evaluation = DEFAULT_EVAL(p);
                       
    for (int i = 0; i < m->count; i++) {

        apply_move(sq, (m->moves+i));
        tmp_evaluation = minimax(sq, SEARCHDEPTH, TOGGLE(p));
        reverse_move(sq, (m->moves+i));
        
        if (BETTER_EVAL(p, tmp_evaluation, best_evaluation)) {
            best_evaluation = tmp_evaluation;
            choice.to = (m->moves+i)->to;
            choice.from = (m->moves+i)->from;
            choice.on_to = (m->moves+i)->on_to;
            choice.on_from = (m->moves+i)->on_from;
            choice.side_effect = (m->moves+i)->side_effect;
        }
    }
    return choice;
}

double minimax(Piece *sq, int depth, Player p)
{
    MoveSet *m = all_legal_moves(sq, p);
    if (is_checkmate(sq, m)) {
        free(m);
        return WHITEBLACK_VAL(p, -1000.00, 1000.00);
    } else if (is_stalemate(sq, m)) {
        free(m);
        return 0.0;
    }

    double tmp_evaluation,
           best_evaluation = DEFAULT_EVAL(p);

    if (depth == 0) {
        best_evaluation = evaluate(sq, m, p);
    } else {
        for (int i = 0; i < m->count; i++) {
            apply_move(sq, m->moves+i);
            tmp_evaluation = minimax(sq, depth-1, TOGGLE(p));
            reverse_move(sq, m->moves+i);

            if (WHITEBLACK_VAL(p, (tmp_evaluation > best_evaluation), (tmp_evaluation < best_evaluation))) {
                best_evaluation = tmp_evaluation;
            }
        }
    }
    free(m->moves);
    free(m);
    return best_evaluation;
}

