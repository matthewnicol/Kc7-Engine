#define either(A,B,C)    (A == B || A == C)
#define VALID(A)    (A >= 0 && A < 64)

static int pawn_moves(Piece[], int, Turn, Move*);
/*@null@*/ static MoveSet* make_moveset(int);
static void kill_moveset(MoveSet*);
static void primary_move(Move*, int, int, Piece, Piece);

void all_legal_moves(Piece sq[], Turn t)
{
    int i;
    MoveSet *m = make_moveset(30);
    assert(m != NULL);
    for (i = 0; i < 64; i++) {
        if (is_pawn[sq[i]]) {
            m->count += pawn_moves(sq, i, t, m->moves+m->count); 
        }
    }

    for (i = 0; i < m->count; i++) {
        printf(
            "SQ %i (piece %i) to SQ %i (piece %i)\n", 
            (int)((m->moves+i)->from),
            (int)((m->moves+i)->on_from),
            (int)((m->moves+i)->to),
            (int)((m->moves+i)->on_to)
        );
    }
    kill_moveset(m);    
}

static int pawn_moves(Piece sq[], int square, Turn t, Move *m)
{
    int i = 0;
    int direction = t? 1 : -1;

    if (VALID(square+(8*direction)) && !sq[square+(8*direction)]) {
        primary_move((m + i++), square, square + (8*direction), sq[square], NO_PIECE);
    }
    if (((RANK_MAP[sq[square]] == 2 && !t) || (RANK_MAP[sq[square]] == 7 && t))
            && i && !sq[square+(16*direction)]) {
        primary_move((m + i++), square, square + (16*direction), sq[square], NO_PIECE);
    }
    return i;

}

static MoveSet *make_moveset(int size)
{
    int i;

    MoveSet *mset = malloc(sizeof (MoveSet) + sizeof(Move) * size);
    if (mset == NULL) return NULL;

    mset->count = 0;
    mset->moves = malloc (sizeof(Move) * size);

    if (mset->moves == NULL) {
        free(mset);
        return NULL;
    }

    Move *m = mset->moves;
    for (i = 0; i < 30; i++) { 

        m[i].from = -1;
        m[i].to = -1;
        m[i].on_from = NO_PIECE;
        m[i].on_to = NO_PIECE;
        m[i].s_effect_from = -1;
        m[i].s_effect_to = -1;
        m[i].s_effect_on_from = NO_PIECE;
        m[i].s_effect_on_to = NO_PIECE;
    }

    return mset;
}

static void kill_moveset(MoveSet* m)
{
    if (m != NULL) {
       if (m->moves != NULL) free(m->moves);
       free(m);
    }
}

static void primary_move(Move *m, int from, int to, Piece on_from, Piece on_to)

{
    m->from = from;
    m->to = to;
    m->on_from = on_from;
    m->on_to = on_to;
}
