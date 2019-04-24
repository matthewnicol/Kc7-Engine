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

    double piece_scores = 0.0;
    double bishop_scores = 0.0;
    double center_scores = 0.0;
    int bishops[] = {0, 0};
    double attack_equivelant_piece = 0.0;
    double attack_better_piece = 0.0;
    double can_move_to = 0.0;

    Move *msq = malloc(sizeof(Move)*30);
    for (int i = 0; i < 64; i++) {
        // Material advantage/disadvantage
        piece_scores += VALUEOF(sq[i]) * (ISBLACK(sq[i]) ? -1 : 1);
        // Count up bishops
        if (sq[i] == WHITE_BISHOP) bishops[0]++;
        if (sq[i] == BLACK_BISHOP) bishops[1]++;
        
        // Moves for each square...
        if (TURNPIECE(p, sq[i])) {
            int k = moves_for_square(sq, i, p, msq);
            for (int j = 0; j < k; j++) {
                // Total moves available
                if (!sq[(msq[j].to)]) {
                    can_move_to += 1* (p == PLAYER_WHITE ? 1 : -1); 
                // Attacks on better pieces
                } else {
                    if (abs(VALUEOF(sq[msq[j].to])) > abs(VALUEOF(sq[msq[j].from]))) {
                        attack_better_piece += 1* (p == PLAYER_WHITE ? 1 : -1); 
                    } else {
                        attack_equivelant_piece += 1* (p == PLAYER_WHITE ? 1 : -1); 

                    }
                }
            
            }

        }
    }
    // Bishop Pair Bonus
    if (bishops[0] == 2) { bishop_scores += .5; }
    if (bishops[1] == 2) { bishop_scores -= .5; }
    free(msq);
    return 
        (double)piece_scores 
        + (double)bishop_scores 
        + ((double)center_scores/200)
        + ((double)attack_equivelant_piece/200) 
        + ((double)attack_better_piece/100)
        + ((double)can_move_to/300);
}

#define SEARCHDEPTH 7
#define BETTER_EVAL(T, A, B) ((T == PLAYER_WHITE && A > B) || (T == PLAYER_BLACK && A < B))

// Alpha = best already explored option along the path to the root for the maximizer
// Beta = best already explored option along the path to the root for the minimizer

Move minimax_choice(Piece *sq, MoveSet *m, Player p)
{
    Move choice;

    double alpha = -1000.00, 
           beta = 1000.00,
           tmp_evaluation;


    if (m->count == 1) { 
        printf("Forced move.\n");
        return *m->moves; 
    }
                       
    for (int i = 0; i < m->count; i++) {
        apply_move(sq, (m->moves+i));
        tmp_evaluation = minimax(sq, SEARCHDEPTH, TOGGLE(p), alpha, beta);
        reverse_move(sq, m->moves+i);
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
        double eval = evaluate(sq, m, p);  
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

