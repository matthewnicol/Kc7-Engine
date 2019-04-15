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
    if (is_checkmate(sq, m)) return WHITEBLACK_VAL(p, -1000.00, 1000.00);
    if (is_stalemate(sq, m)) return 0.0;

    MoveSet *opp_moves = all_legal_moves(sq, p);
    int i; 
    double piece_scores = 0.0;
    double center_scores = 0.0;
    double center_scores_opp = 0.0;
    double choice_scores = (
            WHITEBLACK_VAL(p, (double)m->count, 0.0-m->count) + 
            WHITEBLACK_VAL(p, 0.0-opp_moves->count, (double)opp_moves->count)
    ) / (m->count + opp_moves->count);
    int bishops[] = {0, 0};

    // Material advantage/disadvantage
    for (i = 0; i < 64; i++) {
        piece_scores += PIECE_VALUE_MAP[sq[i]];
        if (is_bishop[sq[i]]) bishops[is_black[sq[i]]]++;
    }
    for (i = 0; i < m->count; i++) {
        center_scores += (square_value[(m->moves+i)->to] * 0.25 * (WHITEBLACK_VAL(p, 1, -1)));
    }
    for (i = 0; i < opp_moves->count; i++) {
        center_scores_opp += (square_value[(opp_moves->moves+i)->to] * 0.25 * (WHITEBLACK_VAL(TOGGLE(p), 1, -1)));
    }
    free(opp_moves->moves);
    free(opp_moves);
    double total_eval =piece_scores 
        + (bishops[0] == 2 ? .5 : 0.0) 
        + (bishops[1] == 2 ? -.5 : 0.0) 
        + center_scores
        + center_scores_opp
        + choice_scores;
#if 0
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
#endif

    return total_eval;
}

#define SEARCHDEPTH 2
#define BETTER_EVAL(T, A, B) ((T == PLAYER_WHITE && A > B) || (T == PLAYER_BLACK && A < B))

// Alpha = best already explored option along the path to the root for the maximizer
// Beta = best already explored option along the path to the root for the minimizer

Move minimax_choice(Piece *sq, MoveSet *m, Player p)
{
    Move choice;
    double alpha = -1000.00, beta = 1000.00;
    double 
        tmp_evaluation,
        best_evaluation = DEFAULT_EVAL(p);

    if (m->count == 1) { return *m->moves; }
                       
    for (int j = 0; j < 3; j++) {
    for (int i = 0; i < m->count; i++) {
        if (j == 0 && !(m->moves+i)->is_checking_move) continue;
        else {
            // Handle captures
            if (j == 1 && sq[(m->moves+i)->to] == NO_PIECE) continue;
            // Handle regular moves
            if (j == 2 && sq[(m->moves+i)->to] != NO_PIECE) continue;
        }

        apply_move(sq, (m->moves+i));
        MoveSet *inner_m = all_legal_moves(sq, TOGGLE(p));
        if (is_checkmate(sq, inner_m)) {
            free(inner_m->moves);
            free(inner_m);
            return *(m->moves+i);
        }
        tmp_evaluation = minimax(sq, inner_m, SEARCHDEPTH, TOGGLE(p), alpha, beta);
        free(inner_m->moves);
        free(inner_m);
        reverse_move(sq, (m->moves+i));
        
        if (BETTER_EVAL(p, tmp_evaluation, best_evaluation) || tmp_evaluation == best_evaluation) {
            best_evaluation = tmp_evaluation;
            alpha = WHITEBLACK_VAL(p, best_evaluation, alpha);
            beta = WHITEBLACK_VAL(p, beta, best_evaluation);
            choice = *(m->moves+i);
        }
    }
    }
    printf("Evaluation of continuation picked: %f\n", best_evaluation);
    return choice;
}

double minimax(Piece *sq, MoveSet *m, int depth, Player p, double alpha, double beta)
{

    double tmp_evaluation,
           best_evaluation = DEFAULT_EVAL(p);

    // Handle zero move cases
    if (is_stalemate(sq, m)) return 0.0;  
    if (depth == 0) return evaluate(sq, m, p);  
    
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < m->count; i++) {
            // Handle checks
            if (j == 0 && !(m->moves+i)->is_checking_move) continue;
            else {
                // Handle captures
                if (j == 1 && sq[(m->moves+i)->to] == NO_PIECE) continue;
                // Handle regular moves
                if (j == 2 && sq[(m->moves+i)->to] != NO_PIECE) continue;
            }
            apply_move(sq, m->moves+i);
            MoveSet *inner_m =  all_legal_moves(sq, TOGGLE(p));
            if ((m->moves+i)->is_checking_move && is_checkmate(sq, inner_m)) {
                free(inner_m->moves);
                free(inner_m);
                return (WHITEBLACK_VAL(p, 1000.0, -1000.0));
            }
            tmp_evaluation = minimax(sq, inner_m, depth-1, TOGGLE(p), alpha, beta);
            free(inner_m->moves);
            free(inner_m);
            if (WHITEBLACK_VAL(p, (tmp_evaluation > best_evaluation), (tmp_evaluation < best_evaluation))) {
                best_evaluation = tmp_evaluation;
            }
            if (p == PLAYER_WHITE) {
                if (tmp_evaluation > beta) {
                    reverse_move(sq, m->moves+i);
                    break;
                }
            } else if (p == PLAYER_BLACK) {
                if (tmp_evaluation < beta) {
                    reverse_move(sq, m->moves+i);
                    break;
                }
            }
            reverse_move(sq, m->moves+i);
        }
    }
    return best_evaluation;
}

