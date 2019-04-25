void printMove(int movenum, Move *m) {
    printf(
        "Mv%2i: SQ %2i (piece %c) to SQ %2i (piece %1c)\n", 
        movenum,
        m->from,
        m->on_from,
        m->to,
        m->on_to
    );
}

void printAllMoves(MoveSet *m) {
    for (int i = 0; i < m->count; i++) {
        printMove(i, m->moves+i);
    }
}

MoveSet *all_legal_moves(Board *b, Turn t)
{
    int i;
    MoveSet *m = make_moveset(60);

    for (i = 0; i < 64; i++) {
        // Add to list of moves we can perform in this position
        m->count += moves_for_square(b->squares, i, t, m->moves+m->count);
        // Record king position if this is our king
        if (m->king_pos == -1 && EITHER(b->squares[i], MYPIECE(t, CASTLING_KING), MYPIECE(t, KING))) m->king_pos = i;
    }
    remove_illegal_moves(b, m);
    return m;
}

static void remove_illegal_moves(Board *b, MoveSet *m)
{
    int k=0;
    for (int i = 0; i < m->count; i++) {
        apply_move(b, m->moves+i);
        int updated_king_pos = ISKING(b->squares[m->moves[i].to]) ? m->moves[i].to : m->king_pos;

        // Keep legal moves - which don't leave our king in check
        if (!square_is_attacked(b, updated_king_pos)) {
            m->moves[k++] = m->moves[i];
        }
        reverse_move(b, m->moves+i);
    }
    m->count = k;
}

int square_is_attacked(Board *b, int square)
{
    int attacker = ISBLACK(b->squares[square]) ? WHITE : BLACK;
    if (square == -1) return 0;
    Move *m = malloc(sizeof(Move)*100);
    assert (m != NULL);
    for (int i = knight_moves(b->squares, square, m)-1; i >= 0; i--) {
        if (b->squares[(m+i)->to] == MYPIECE(attacker, KNIGHT)) {
            free(m);
            return 1;
        }
    }
    for (int i = linewise_piece_moves(b->squares, square, 1, 1, m)-1; i >= 0; i--) {
        if (b->squares[(m+i)->to] == MYPIECE(attacker, QUEEN)) {
            free(m);
            return 1;
        }
        if (FILE_MAP[(m+i)->to] == FILE_MAP[(m+i)->from] || RANK_MAP[(m+i)->to] == RANK_MAP[(m+i)->from]) {
            if (b->squares[(m+i)->to] == MYPIECE(attacker, ROOK) || b->squares[(m+i)->to] == MYPIECE(attacker, CASTLING_ROOK)) {
                free(m);
                return 1;
            }
        } else {
            if (b->squares[(m+i)->to] == MYPIECE(attacker, BISHOP)) {
                free(m);
                return 1;
            }
        }
    }
    for (int i = pawn_captures(b->squares, square, TOGGLE(attacker), m); i >= 0; i--) {
        if (b->squares[(m+i)->to] == MYPIECE(attacker, PAWN) || b->squares[(m+i)->to] == MYPIECE(attacker, EP_PAWN)) {
            free(m);
            return 1;
        }
    }
    free(m);
    return 0;
}

static int moves_for_square(Piece *sq, int square, Turn t, Move *m)
{
    if (!TURNPIECE(t, sq[square])) return 0;
    if (ISPAWN(sq[square])) {
        int movec = 0;
        movec += pawn_advances(sq, square, t, m+movec); 
        movec += pawn_captures(sq, square, t, m+movec);
        movec += pawn_ep_captures(sq, square, t, m+movec);
        movec += pawn_promotions(sq, square, t, m+movec);
        return movec;
    } else if (ISKNIGHT(sq[square])) {
        return knight_moves(sq, square, m);
    } else if (ISKING(sq[square])) {
        int movec = 0;
        movec += king_moves(sq, square, m);
        movec += king_castles(sq, square, m+movec);
        return movec;
    } else if (ISBISHOP(sq[square])) {
        return linewise_piece_moves(sq, square, 1, 0, m);
    } else if (ISROOK(sq[square])) {
        return linewise_piece_moves(sq, square, 0, 1, m);
    } else if (ISQUEEN(sq[square])) {
        return linewise_piece_moves(sq, square, 1, 1, m);
    }
    return 0;
}

static MoveSet *make_moveset(int size)
{
    MoveSet *mset = malloc(sizeof (MoveSet));
    if (mset == NULL) return NULL;
    mset->king_pos = -1;
    mset->count = 0;
    mset->moves = malloc (sizeof(Move) * size);

    if (mset->moves == NULL) {
        free(mset);
        return NULL;
    }
    
    mset->moves->from = -1;
    mset->moves->to = -1;
    mset->moves->on_from = -1;
    mset->moves->on_to = -1;
    mset->moves->side_effect = 0;
    mset->moves->is_checking_move = 0;
    for (int i = 0; i < mset->count; i++) {
        assert(mset->moves+i != NULL);
        mset->moves[i].from = -1;
        mset->moves[i].to = -1;
        mset->moves[i].on_from = -1;
        mset->moves[i].on_to = -1;
    }

    return mset;
}

static void basic_move(Move *m, int from, int to, Piece on_from, Piece on_to)
{
    assert(m != NULL);
    assert(from > -1 && from < 64);
    assert(to > -1 && to < 64);
    assert(ISPIECE(on_from));
    m->from = from;
    m->to = to;
    m->on_from = on_from;
    m->on_to = on_to;
    m->side_effect = 0;
    m->is_checking_move = 0;
}

static void move_with_side_effect(Move *m, int from, int to, Piece on_from, Piece on_to, MoveSideEffect mse)
{
    basic_move(m, from, to, on_from, on_to);
    m->side_effect = mse;
}
