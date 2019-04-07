#define either(A,B,C)    (A == B || A == C)
#define VALID(A)    (A >= 0 && A < 64)

static int pawn_advances(Piece[], int, Turn, Move*);
static int pawn_captures(Piece[], int, Turn, Move*);
static int pawn_ep_captures(Piece[], int, Turn, Move*);
static int pawn_promotions(Piece[], int, Turn, Move*);
static int knight_moves(Piece[], int, Move*);
/*@null@*/ static MoveSet* make_moveset(int);
static void kill_moveset(MoveSet*);
static void primary_move(Move*, int, int, Piece, Piece);
static void side_effect_move(Move*, MoveSideEffect);

void all_legal_moves(Piece sq[], Turn t)
{
    int i;
    MoveSet *m = make_moveset(30);
    assert(m != NULL);
    for (i = 0; i < 64; i++) {
        if (is_pawn[sq[i]]) {
            m->count += pawn_advances(sq, i, t, m->moves+m->count); 
            m->count += pawn_captures(sq, i, t, m->moves+m->count);
            m->count += pawn_ep_captures(sq, i, t, m->moves+m->count);
            m->count += pawn_promotions(sq, i, t, m->moves+m->count);
        } else if (is_knight[sq[i]]) {
            m->count += knight_moves(sq, i, m->moves+m->count);
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

static int pawn_advances(Piece sq[], int square, Turn t, Move *m)
{
    int i = 0, direction = t? 1 : -1;
    int on_home_rank = (RANK_MAP[sq[square]] == 2 && t) || (RANK_MAP[sq[square]] == 7 && !t);
    int on_promote_rank = (RANK_MAP[sq[square]] == 7 && t) || (RANK_MAP[sq[square]] == 2 && !t);
    int can_move_1 = (VALID(square+(8*direction)) && !sq[square+(8*direction)] && !on_promote_rank);
    int can_move_2 = (
        can_move_1
        && VALID(square+(16*direction)) 
        && on_home_rank
        && !sq[square+(16*direction)]
    );

    if (can_move_1) {
        primary_move((m + i++), square, square + (8*direction), sq[square], NO_PIECE);
    }

    if (can_move_2) {
        primary_move((m + i++), square, square + (16*direction), sq[square], NO_PIECE);
    } 
    return i;
}

static int pawn_captures(Piece sq[], int square, Turn t, Move *m)
{
    int i = 0, direction = t? 1 : -1;
    if (different_team(sq, square, square+(9*direction))) {
        primary_move((m + i++), square, square + (9*direction), sq[square], sq[square+(9*direction)]);
    }
    if (different_team(sq, square, square+(7*direction))) {
        primary_move((m + i++), square, square + (7*direction), sq[square], sq[square+(7*direction)]);
    }
    return i;
}

static int pawn_ep_captures(Piece sq[], int square, Turn t, Move *m)
{
    int i = 0, direction = t? 1 : -1;
    Piece them = t? WHITE_EP_PAWN : BLACK_EP_PAWN;
    char ep1_file = t? 'a' : 'h';
    char ep2_file = t? 'h' : 'a';

    if (sq[square+(-1*direction)] == them && FILE_MAP[square+(-1*direction)] != ep1_file) {
        primary_move((m + i++), square, square + (9*direction), sq[square], sq[square+(9*direction)]);
        side_effect_move((m + i-1), EP_CAPTURE);
    }
    if (sq[square+(1*direction)] == them && FILE_MAP[square+(1*direction)] != ep2_file) {
        primary_move((m + i++), square, square + (7*direction), sq[square], sq[square+(7*direction)]);
        side_effect_move((m + i-1), EP_CAPTURE);
    }
    return i;
}

static int pawn_promotions(Piece sq[], int square, Turn t, Move *m)
{

    static Piece promotions[][4] = {
        {BLACK_QUEEN, BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP},
        {WHITE_QUEEN, WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP},
    };


    int i = 0, j, direction = t? 1 : -1;
    int on_promote_rank = (RANK_MAP[sq[square]] == 7 && t) || (RANK_MAP[sq[square]] == 2 && !t);
    if (on_promote_rank && !sq[square+(8*direction)])
    {
        for (j = 0; j < 4; j++) {
            primary_move((m + i++), square, square + (8*direction), sq[square], promotions[t][j]);
            side_effect_move((m + i-1), PROMOTION);
        }
    }
    return i;

}

static int knight_moves(Piece sq[], int square, Move *m)
{
    static int knight_diffs[][2] = {
        {-1, 2}, {1, 2}, {-1, -2}, {1, -2},
        {2, -1}, {2, 1}, {-2, -1}, {-2, 1},
    };

    static char bad_knight_jumps [][2] = {
        {'a', 'h'}, {'a', 'g'}, {'b', 'h'},
        {'h', 'a'}, {'g', 'a'}, {'h', 'b'}
    };

    int i = 0, j;

    for (j = 0; j < 8; j++) {
        int sqto = square + knight_diffs[j][0]*8 + knight_diffs[j][1];
        if (VALID(sqto) && (!sq[sqto] || different_team(sq, square, sqto))) {
            int k, clipping = 0;
            for (k = 0; k < 6; k++) {
                if (FILE_MAP[sqto] == bad_knight_jumps[j][0] && FILE_MAP[square] == bad_knight_jumps[j][1]) {
                    clipping = 1;
                    break;
                }
            }
            if (!clipping) primary_move((m+i++), square, sqto, sq[square], sq[sqto]); 
        }
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
        m[i].side_effect = NO_S_EFFECT;
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

static void side_effect_move(Move *m, MoveSideEffect mse)
{
    m->side_effect = mse;
}
