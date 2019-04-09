#define either(A,B,C)    (A == B || A == C)
#define VALID(A)    (A >= 0 && A < 64)

static int pawn_advances(Piece[], int, Turn, Move*);
static int pawn_captures(Piece[], int, Turn, Move*);
static int pawn_ep_captures(Piece[], int, Turn, Move*);
static int pawn_promotions(Piece[], int, Turn, Move*);
static int knight_moves(Piece[], int, Move*);
static int king_moves(Piece[], int, Move*);
static int king_castles(Piece[], int, Move*);
static int linewise_piece_moves(Piece[], int, int, int, Move*);
/*@null@*/ static MoveSet* make_moveset(int);
static void kill_moveset(MoveSet*);
static void basic_move(Move*, int, int, Piece, Piece);
static void move_with_side_effect(Move*, int, int, Piece, Piece, MoveSideEffect);
int moves_for_square(Piece[], int, Turn, MoveSet*);

MoveSet *all_legal_moves(Piece sq[], Turn t)
{
    int i;
    MoveSet *m = make_moveset(30);
    assert(m != NULL);
    for (i = 0; i < 64; i++) {
        if ((t && is_white[sq[i]]) || (!t && is_black[sq[i]])) continue;
        if (is_pawn[sq[i]]) {
            m->count += pawn_advances(sq, i, t, m->moves+m->count); 
            m->count += pawn_captures(sq, i, t, m->moves+m->count);
            m->count += pawn_ep_captures(sq, i, t, m->moves+m->count);
            m->count += pawn_promotions(sq, i, t, m->moves+m->count);
        } else if (is_knight[sq[i]]) {
            m->count += knight_moves(sq, i, m->moves+m->count);
        } else if (is_king[sq[i]]) {
            m->count += king_moves(sq, i, m->moves+m->count);
            m->count += king_castles(sq, i, m->moves+m->count);
        } else if (is_bishop[sq[i]]) {
            m->count += linewise_piece_moves(sq, i, 1, 0, m->moves+m->count);
        } else if (is_rook[sq[i]]) {
            m->count += linewise_piece_moves(sq, i, 0, 1, m->moves+m->count);
        } else if (is_queen[sq[i]]) {
            m->count += linewise_piece_moves(sq, i, 1, 1, m->moves+m->count);
        }
    }
    
    for (i = 0; i < m->count; i++) {
    
    }

    for (i = 0; i < m->count; i++) {
        printf(
            "Mv%2i: SQ %i (piece %i) to SQ %i (piece %i) %s\n", 
            i,
            (int)((m->moves+i)->from),
            (int)((m->moves+i)->on_from),
            (int)((m->moves+i)->to),
            (int)((m->moves+i)->on_to),
            (m->moves+i)->on_to ? "(CAPTURE)" : ""
        );
    }
    return m;
}

int moves_for_square(Piece sq[], int square, Turn t, MoveSet *m)
{
    if ((t && is_white[sq[square]]) || (!t && is_black[sq[square]])) return 0;
    if (is_pawn[sq[square]]) {
        int movec = 0;
        movec += pawn_advances(sq, square, t, m->moves+movec); 
        movec += pawn_captures(sq, square, t, m->moves+movec);
        movec += pawn_ep_captures(sq, square, t, m->moves+movec);
        movec += pawn_promotions(sq, square, t, m->moves+movec);
        return movec;
    } else if (is_knight[sq[square]]) {
        return knight_moves(sq, square, m->moves);
    } else if (is_king[sq[square]]) {
        int movec = 0;
        movec += king_moves(sq, square, m->moves);
        movec += king_castles(sq, square, m->moves+movec);
        return movec;
    } else if (is_bishop[sq[square]]) {
        return linewise_piece_moves(sq, square, 1, 0, m->moves);
    } else if (is_rook[sq[square]]) {
        return linewise_piece_moves(sq, square, 0, 1, m->moves);
    } else if (is_queen[sq[square]]) {
        return linewise_piece_moves(sq, square, 1, 1, m->moves);
    }
    return 0;
}

static int pawn_advances(Piece sq[], int square, Turn t, Move *m)
{
    int i = 0, direction = t == PLAYER_BLACK? 1 : -1;
    int on_home_rank = (RANK_MAP[square] == 2 && t == PLAYER_WHITE) || (RANK_MAP[square] == 7 && t == PLAYER_BLACK);
    int on_promote_sq = (RANK_MAP[square] == 7 && t == PLAYER_WHITE) || (RANK_MAP[square] == 2 && t == PLAYER_BLACK);
    int can_move_1 = (VALID(square+(8*direction)) && !sq[square+(8*direction)] && !on_promote_sq);
    int can_move_2 = (
        can_move_1
        && VALID(square+(16*direction)) 
        && on_home_rank
        && !sq[square+(16*direction)]
    );

    if (can_move_1) {
        basic_move((m + i++), square, square + (8*direction), sq[square], NO_PIECE);
    }

    if (can_move_2) {
        basic_move((m + i++), square, square + (16*direction), sq[square], NO_PIECE);
    } 
    return i;
}

static int pawn_captures(Piece sq[], int square, Turn t, Move *m)
{
    int i = 0, direction = t? 1 : -1;
    if (different_team(sq, square, square+(9*direction))) {
        if (!(FILE_MAP[square] == 'a' && FILE_MAP[square+(9*direction)] == 'h') &&
         !(FILE_MAP[square] == 'h' && FILE_MAP[square+(9*direction)] == 'a'))
        basic_move((m + i++), square, square + (9*direction), sq[square], sq[square+(9*direction)]);
    }
    if (different_team(sq, square, square+(7*direction))) {
        if (!(FILE_MAP[square] == 'a' && FILE_MAP[square+(7*direction)] == 'h') &&
         !(FILE_MAP[square] == 'h' && FILE_MAP[square+(7*direction)] == 'a'))
        basic_move((m + i++), square, square + (7*direction), sq[square], sq[square+(7*direction)]);
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
        move_with_side_effect(
            (m + i++), 
            square, 
            square + (9*direction), 
            sq[square], 
            sq[square+(9*direction)],
            EP_CAPTURE);
    }
    if (sq[square+(1*direction)] == them && FILE_MAP[square+(1*direction)] != ep2_file) {
        move_with_side_effect(
            (m + i++), 
            square, 
            square + (7*direction), 
            sq[square], 
            sq[square+(7*direction)],
            EP_CAPTURE);
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
            move_with_side_effect(
                (m + i++), 
                square, 
                square + (8*direction), 
                sq[square], 
                promotions[t][j],
                PROMOTION);
        }
    }
    return i;

}

static int knight_moves(Piece sq[], int square, Move *m)
{
    static int knight_diffs[][2] = {
        {-1, 2}, {1, 2}, {-1, -2}, {1, -2}, {2, -1}, {2, 1}, {-2, -1}, {-2, 1},
    };

    static char bad_knight_jumps [][2] = {
        {'a', 'h'}, {'a', 'g'}, {'b', 'h'}, {'h', 'a'}, {'g', 'a'}, {'h', 'b'}
    };

    int i = 0, j;

    for (j = 0; j < 8; j++) {
        int sqto = square + (knight_diffs[j][0]*8) + (knight_diffs[j][1]);
        if (!VALID(sqto)) continue;
        if (!sq[sqto] || different_team(sq, square, sqto)) {
            int k, clipping = 0;
            for (k = 0; k < 6; k++) {
                if (FILE_MAP[sqto] == bad_knight_jumps[k][0] && FILE_MAP[square] == bad_knight_jumps[k][1]) {
                    clipping = 1;
                    break;
                }
            }
            if (!clipping) basic_move((m+i++), square, sqto, sq[square], sq[sqto]); 
        }
    } 
    return i;
}

static int king_moves(Piece sq[], int square, Move *m)
{
    static int king_diffs[][2] = {
        {0, 1}, {0, -1}, {1, 1}, {1, -1}, {1, 0}, {-1, 1}, {-1, -1}, {-1, 0}
    };

    int i = 0, j;

    for (j = 0; j < 8; j++) {
        int sqto = square + king_diffs[j][0]*8 + king_diffs[j][1];
        if (VALID(sqto) && (!sq[sqto] || different_team(sq, square, sqto))) {
            basic_move((m+i++), square, sqto, sq[square], sq[sqto]);
        }
    }
    return i;
}

static int linewise_piece_moves(Piece sq[], int square, int diagonal, int upanddown, Move *m)
{
    int i = 0, j, k;
    int blockaded[] = {0, 0, 0, 0, 0, 0, 0, 0};
    char onfile[] = {
        FILE_MAP[square], FILE_MAP[square], FILE_MAP[square], FILE_MAP[square], 
        FILE_MAP[square], FILE_MAP[square], FILE_MAP[square], FILE_MAP[square]
    };
    for (k = 1; k < 8; k++) {
        int directions[8] = {
            square + k,
            square - k,
            square - (k * 8),
            square + (k * 8),
            square + (k * 8) + k,
            square - (k * 8) - k,
            square - (k * 8) + k,
            square + (k * 8) - k,
        };
        for (j = 0; j < 8; j++) {
            if (!VALID(directions[j])) continue;
            if (!diagonal && j > 3) continue;
            if (!upanddown && j < 4) continue;
            if (blockaded[j]) continue;

            if (onfile[j] == 'a' && FILE_MAP[directions[j]] == 'h') {
                blockaded[j] = 1;
                continue;
            }
            if (onfile[j] == 'h' && FILE_MAP[directions[j]] == 'a') {
                blockaded[j] = 1;
                continue;
            }

            onfile[j] = FILE_MAP[directions[j]];

            blockaded[j] = sq[directions[j]] > 0;
            if ((blockaded[j] && different_team(sq, square, directions[j])) || !blockaded[j]) {
                basic_move((m+i++), square, directions[j], sq[square], sq[directions[j]]);
            }
        }
    }
    return i;
}


static int king_castles(Piece sq[], int square, Move *m)
{
    static Piece castling_king[] = {BLACK_CASTLING_KING, WHITE_CASTLING_KING};
    static Piece castling_rook[] = {BLACK_CASTLING_ROOK, WHITE_CASTLING_ROOK};
    static Piece moved_king[] = {BLACK_KING, WHITE_KING};
    int piece_index = is_white[sq[square]];
    int i = 0;
    int ks = is_black[sq[square]] ? 4 : 60;

    if (!(square == ks && sq[square] != castling_king[piece_index]))
        return 0;

    if (sq[square+3] == castling_rook[piece_index] && !sq[square+1] && !sq[square+2]) {
        move_with_side_effect((m+i++), square, square+2, moved_king[piece_index], NO_PIECE, KS_CASTLE);
    }
    if (sq[square-4] == castling_rook[piece_index] && !sq[square-1] && !sq[square-2] && !sq[square-3]) {
        move_with_side_effect((m+i++), square, square+2, moved_king[piece_index], NO_PIECE, QS_CASTLE);
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

static void basic_move(Move *m, int from, int to, Piece on_from, Piece on_to)
{
    assert(from >= 0 && from < 64);
    assert(to >= 0 && to < 64);
    assert((int)on_from >= 0 && on_from < 22);
    assert((int)on_to >= 0 && on_to < 22);
    m->from = from;
    m->to = to;
    m->on_from = on_from;
    m->on_to = on_to;
}

static void move_with_side_effect(Move *m, int from, int to, Piece on_from, Piece on_to, MoveSideEffect mse)
{
    basic_move(m, from, to, on_from, on_to);
    m->side_effect = mse;
}
