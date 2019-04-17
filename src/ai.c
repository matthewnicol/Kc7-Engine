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


//static int square_value[] = {
//    0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,  75,  75,  75,  75,   0,   0,
//    0,   0,  75, 100, 100,  75,   0,   0,
//    0,   0,  75, 100, 100,  75,   0,   0,
//    0,   0,  75,  75,  75,  75,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0
//};

double evaluate(Piece *sq, MoveSet *m, Player p) 
{
    if (is_checkmate(sq, m)) return WHITEBLACK_VAL(p, -1000.00, 1000.00);
    if (is_stalemate(sq, m)) return 0.0;

//    MoveSet *opp_moves = all_legal_moves(sq, p);
//    int i; 
    double piece_scores = 0.0;
//    double center_scores = 0.0;
//    double center_scores_opp = 0.0;
//    double choice_scores = (
//            WHITEBLACK_VAL(p, (double)m->count, 0.0-m->count) + 
//            WHITEBLACK_VAL(p, 0.0-opp_moves->count, (double)opp_moves->count)
//    ) / (m->count + opp_moves->count);
//    int bishops[] = {0, 0};

    // Material advantage/disadvantage
    for (int i = 0; i < 64; i++) {
        piece_scores += PIECE_VALUE_MAP[sq[i]];
 //       if (is_bishop[sq[i]]) bishops[is_black[sq[i]]]++;
    }
//    for (i = 0; i < m->count; i++) {
//        center_scores += (square_value[(m->moves+i)->to] * 0.025 * (WHITEBLACK_VAL(p, 1, -1)));
//    }
//    for (i = 0; i < opp_moves->count; i++) {
//        center_scores_opp += (square_value[(opp_moves->moves+i)->to] * 0.25 * (WHITEBLACK_VAL(TOGGLE(p), 1, -1)));
//    }
//    free(opp_moves->moves);
//    free(opp_moves);
//    double total_eval = piece_scores 
//        + (bishops[0] == 2 ? .5 : 0.0) 
//        + (bishops[1] == 2 ? -.5 : 0.0) 
//        + center_scores
//        + center_scores_opp
//        + choice_scores;
//    return total_eval;
    return (double)piece_scores;
}

#define SEARCHDEPTH 5
#define BETTER_EVAL(T, A, B) ((T == PLAYER_WHITE && A > B) || (T == PLAYER_BLACK && A < B))

// Alpha = best already explored option along the path to the root for the maximizer
// Beta = best already explored option along the path to the root for the minimizer

Move minimax_choice(Piece *sq, MoveSet *m, Player p)
{
    Move choice;

    double alpha = -1001.00, 
           beta = 1001.00,
           tmp_evaluation;


    if (m->count == 1) { 
        printf("Forced move.\n");
        return *m->moves; 
    }
                       
    for (int i = 0; i < m->count; i++) {
        apply_move(sq, (m->moves+i));
        tmp_evaluation = minimax(sq, SEARCHDEPTH, TOGGLE(p), alpha, beta);
        reverse_move(sq, m->moves+i);
        printf("Evaluation available: %f (%i to %i)\n", tmp_evaluation, (m->moves+i)->from, (m->moves+i)->to);
        if (p == PLAYER_WHITE && tmp_evaluation > alpha) {
            alpha = tmp_evaluation;
            choice = m->moves[i];
        }
        if (p == PLAYER_BLACK && tmp_evaluation < beta) {
            beta = tmp_evaluation;
            choice = m->moves[i];
        }
    }
    printf("Evaluation of continuation picked: %f\n", p == PLAYER_WHITE ? alpha : beta);
    return choice;
}

double minimax(Piece *sq, int depth, Player p, double alpha, double beta)
{
    MoveSet *m = all_legal_moves(sq, p);
    if (depth == 0 || m->count == 0) {
        double eval =evaluate(sq, m, p);  
        free(m->moves);
        free(m);
        return eval;
    }
    
    for (int i = 0; i < m->count; i++) {
        apply_move(sq, m->moves+i);
        double tmp_evaluation = minimax(sq, depth-1, TOGGLE(p), alpha, beta);

        if (p == PLAYER_WHITE && tmp_evaluation > alpha) {
            alpha = tmp_evaluation;
        }
        if (p == PLAYER_BLACK && tmp_evaluation < beta) {
            beta = tmp_evaluation;
        }

        reverse_move(sq, m->moves+i);
        if (beta >= alpha) {
            break;
        }
    } 
    free(m->moves);
    free(m);
    return p == PLAYER_WHITE ? alpha : beta;
}

