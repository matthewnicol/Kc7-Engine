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


int square_value[] = {
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,  75,  75,  75,  75,   0,   0,
    0,   0,  75, 100, 100,  75,   0,   0,
    0,   0,  75, 100, 100,  75,   0,   0,
    0,   0,  75,  75,  75,  75,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0
};

double evaluate(Piece *sq, MoveSet *m, Player p) 
{
    MoveSet *m_opp = all_legal_moves(sq, p);
    if (is_checkmate(sq, m)) {
        return WHITEBLACK_VAL(p, -1000.00, 1000.00);
    } else if (is_stalemate(sq, m)) {
        return 0.0;
    }
    int i; 
    double piece_scores = 0.0;
    double center_scores = 0.0;
    double center_scores_opp = 0.0;
    double choice_scores = (
            WHITEBLACK_VAL(p, m->count, 0-m->count) + 
            WHITEBLACK_VAL(p, 0-m_opp->count, m_opp->count)
    ) / (m->count + m_opp->count);
    int bishops[] = {0, 0};
    // Close game
    // Material advantage/disadvantage
    for (i = 0; i < 64; i++) {
        piece_scores += PIECE_VALUE_MAP[sq[i]];
        if (is_bishop[sq[i]]) bishops[is_black[sq[i]]]++;
    }
    for (i = 0; i < m->count; i++) {
        center_scores += (square_value[(m->moves+i)->to] * 0.25 * (WHITEBLACK_VAL(p, 1, -1)));
    }
    for (i = 0; i < m_opp->count; i++) {
        center_scores_opp += (square_value[(m_opp->moves+i)->to] * 0.25 * (WHITEBLACK_VAL(TOGGLE(p), 1, -1)));
    }
    double total_eval =piece_scores 
        + (bishops[0] == 2 ? .5 : 0.0) 
        + (bishops[1] == 2 ? -.5 : 0.0) 
        + center_scores
        + center_scores_opp
        + choice_scores;

    printBoard(sq);
    printf("Deciding: %s\n", WHITEBLACK_VAL(p, "WHITE", "BLACK"));
    printf("Piece Scores (AGGREGATED): %f\n", piece_scores);
    printf("Choice Scores (AGGREGATED): %f\n", choice_scores);
    printf("Center Scores (%s): %f\n", WHITEBLACK_VAL(p, "WHITE", "BLACK"), center_scores);
    printf("Center Scores (%s): %f\n", WHITEBLACK_VAL(p, "BLACK", "WHITE"), center_scores_opp);
    printf("Bishop Scores (WHITE): %f\n", bishops[0] == 2 ? .5 : 0.0);
    printf("Bishop Scores (BLACK): %f\n", bishops[1] == 2 ? -.5 : 0.0);
    printf("\nTotal Evaluation: %f\n", total_eval);

    exit(0);
    return piece_scores 
        + (bishops[0] == 2 ? .5 : 0.0) 
        + (bishops[1] == 2 ? -.5 : 0.0) 
        + center_scores
        + center_scores_opp
        + choice_scores;
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
    printf("Evaluation of continuation picked: %f\n", best_evaluation);
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

