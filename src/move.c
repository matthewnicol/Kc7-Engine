void printMove(int movenum, Move *m) {
    printf(
        "Mv%2i: SQ %i (piece %i) to SQ %i (piece %i) %s\n", 
        movenum,
        (int)((m)->from),
        (int)((m)->on_from),
        (int)((m)->to),
        (int)((m)->on_to),
        (m)->on_to ? "(CAPTURE)" : ""
    );
}

void printAllMoves(MoveSet *m) {
    for (int i = 0; i < m->count; i++) {
        printMove(i, m->moves+i);
    }
}

MoveSet *all_legal_moves(Piece *sq, Turn t)
{
    int i;
    MoveSet *m = make_moveset(60);

    for (i = 0; i < 64; i++) {
        // Add to list of moves we can perform in this position
        m->count += moves_for_square(sq, i, t, m->moves+m->count);
        // Record king position if this is our king
        if (m->king_pos == -1 && EITHER(sq[i], MYPIECE(t, CASTLING_KING), MYPIECE(t, KING))) m->king_pos = i;
    }

    remove_illegal_moves(sq, m);
    return m;
}

static void remove_illegal_moves(Piece *sq, MoveSet *m)
{
    int k=0;
    for (int i = 0; i < m->count; i++) {
        apply_move(sq, m->moves+i);
        int updated_king_pos = is_king[sq[m->moves[i].to]] ? m->moves[i].to : m->king_pos;

        // Keep legal moves - which don't leave our king in check
        if (!square_is_attacked(sq, updated_king_pos)) {
            m->moves[k++] = m->moves[i];
        }
        reverse_move(sq, m->moves+i);
    }
    m->count = k;
}

int square_is_attacked(Piece *sq, int square)
{
    int j, k, attacker = is_black[sq[square]] ? PLAYER_WHITE : PLAYER_BLACK;
    if (square == -1) return 0;
    Move *m = malloc(sizeof(Move)*100);
    for (int i = 0; i < 64; i++) {
        if (i == square) continue;
        k = moves_for_square(sq, i, attacker, m);
        for (j = 0; j < k &&  m+j != NULL && m[j].to != square; j++); 
        if (j < k) break;
    }
    free(m);
    return j < k;
}

static int moves_for_square(Piece *sq, int square, Turn t, Move *m)
{
    if ((t && is_white[sq[square]]) || (!t && is_black[sq[square]])) return 0;
    if (is_pawn[sq[square]]) {
        int movec = 0;
        movec += pawn_advances(sq, square, t, m+movec); 
        movec += pawn_captures(sq, square, t, m+movec);
        movec += pawn_ep_captures(sq, square, t, m+movec);
        movec += pawn_promotions(sq, square, t, m+movec);
        return movec;
    } else if (is_knight[sq[square]]) {
        return knight_moves(sq, square, m);
    } else if (is_king[sq[square]]) {
        int movec = 0;
        movec += king_moves(sq, square, m);
        movec += king_castles(sq, square, m+movec);
        return movec;
    } else if (is_bishop[sq[square]]) {
        return linewise_piece_moves(sq, square, 1, 0, m);
    } else if (is_rook[sq[square]]) {
        return linewise_piece_moves(sq, square, 0, 1, m);
    } else if (is_queen[sq[square]]) {
        return linewise_piece_moves(sq, square, 1, 1, m);
    }
    return 0;
}

static MoveSet *make_moveset(int size)
{
    MoveSet *mset = malloc(sizeof (MoveSet));
    if (mset == NULL) return NULL;
    mset->king_pos = -1;
    mset->king_pos_opp = -1;
    mset->count = 0;
    mset->moves = malloc (sizeof(Move) * size);

    if (mset->moves == NULL) {
        free(mset);
        return NULL;
    }

    return mset;
}

static void basic_move(Move *m, int from, int to, Piece on_from, Piece on_to)
{
    assert(m != NULL);
    m->from = from;
    m->to = to;
    m->on_from = on_from;
    m->on_to = on_to;

    //NOTE: This does not account for discovered checks or promotion checks
    if (is_king[on_to]) m->is_checking_move = 1; 
}

static void move_with_side_effect(Move *m, int from, int to, Piece on_from, Piece on_to, MoveSideEffect mse)
{
    basic_move(m, from, to, on_from, on_to);
    m->side_effect = mse;
}
