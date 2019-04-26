char board_wrapping_files[][2] = {{'a', 'h'}, {'h', 'a'}, {'\0', '\0'}};
#define WRAPS(A, B) (matches_pair(FILE_MAP[A], FILE_MAP[B], board_wrapping_files))

static int pawn_advances(Piece *sq, int square, Turn t, Move *m)
{
    int i = 0;
    int on_home_rank = COLOURCOND(RANK_MAP[square], t, 2, 7);

    int dst[] = {square+(8*DIRECTION(t)), square+(16*DIRECTION(t))};
    for (int k = 0; k < 2; k++) {
        if (!VALID(dst[k]) || sq[dst[k]] != ' ' || (k == 1 && !on_home_rank)) { return i; }
        basic_move((m+i++), square, dst[k]);
    }
    return i;
}

int pawn_captures(Piece *sq, int square, Turn t, Move *m)
{
    int i = 0;
    int differentials[] = {7, 9};
    for (int k = 0; k < 2; k++) {
        int dst = square+(differentials[k]*DIRECTION(t));
        if (VALID(dst) && ENEMIES(sq, square, dst) && !WRAPS(square, dst)) {
            basic_move((m + i++), square, dst);
        }
    }
    return i;
}

static int pawn_ep_captures(Piece *sq, int square, Turn t, Move *m)
{
    int i = 0, direction = t? 1 : -1;
    int differentials[] = {square+direction*7, square+direction*9};
    int epsq[] = {square-1*direction, square+1*direction};
    for (int k = 0; k < 2; k++) { 
        if (VALID(differentials[k]) && sq[epsq[k]] == THEIRPIECE(t, EP_PAWN) && !WRAPS(square, epsq[k])) {
            move_with_side_effect((m + i++), square, differentials[k], EP_CAPTURE); 
        }
    }
    return i;
}

static int pawn_promotions(Piece *sq, int square, Turn t, Move *m)
{

    static Piece promotions[][4] = {
        {BLACK_QUEEN, BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP},
        {WHITE_QUEEN, WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP},
    };


    int i = 0, j, direction = t? 1 : -1;
    int on_promote_rank = (RANK_MAP[square] == 7 && t) || (RANK_MAP[square] == 2 && !t);
    if (on_promote_rank && !sq[square+DIRECTION(t)])
    {
        for (j = 0; j < 4; j++) {
            move_with_side_effect(
                (m + i++), 
                square, 
                square + (DIRECTION(t)*8), 
                PROMOTION);
        }
    }
    return i;

}

int knight_moves(Piece *sq, int square, Move *m)
{
    static int knight_diffs[] = {
        -6, 6, -10, 10, -15, 15, -17, 17
    };

    static char bad_knight_jumps [][2] = {
        {'a', 'h'}, {'a', 'g'}, {'b', 'h'}, {'h', 'a'}, {'g', 'a'}, {'h', 'b'}, {'\0', '\0'}
    };

    int i = 0, j;

    for (j = 0; j < 8; j++) {
        int sqto = square + knight_diffs[j];
        if (
                !VALID(sqto) 
               || matches_pair(FILE_MAP[sqto], FILE_MAP[square], bad_knight_jumps)
               || same_team(sq, square, sqto)
                ) continue;
            basic_move((m+i++), square, sqto); 
    } 
    return i;
}

static int king_moves(Piece *sq, int square, Move *m)
{
    static int king_diffs[][2] = {
        {0, 1}, {0, -1}, {1, 1}, {1, -1}, {1, 0}, {-1, 1}, {-1, -1}, {-1, 0}
    };

    int i = 0, j;

    for (j = 0; j < 8; j++) {
        int sqto = square + king_diffs[j][0]*8 + king_diffs[j][1];
        if (VALID(sqto) && (sq[sqto] == ' ' || different_team(sq, square, sqto))) {
            if (FILE_MAP[sqto] == 'h' && FILE_MAP[square] == 'a') continue;
            if (FILE_MAP[sqto] == 'a' && FILE_MAP[square] == 'h') continue;
            basic_move((m+i++), square, sqto);
        }
    }
    return i;
}

int linewise_piece_moves(Piece *sq, int square, int diagonal, int upanddown, Move *m)
{
    int i = 0;
    for (int j = 0; j < 8; j++) {
        for (int k = 1; k < 8; k++) {
            int dest;
            switch (j) {
                case 0: dest = square + k; break;
                case 1: dest = square - k; break;
                case 2: dest = square - (k * 8); break;
                case 3: dest = square + (k * 8); break;
                case 4: dest = square + (k * 8) + k; break;
                case 5: dest = square - (k * 8) - k; break;
                case 6: dest = square - (k * 8) + k; break;
                case 7: dest = square + (k * 8) - k; break;
            }

            if (
                    !VALID(dest) 
                    || (!diagonal && (j > 3)) 
                    || (!upanddown && (j < 4))
                    || (FILE_MAP[dest] == 'h' && EITHER3(j, 1, 5, 7))
                    || (FILE_MAP[dest] == 'a' && EITHER3(j, 0, 4, 6)) 
                    || same_team(sq, square, dest)
                    ) {
                break;
            }
            basic_move((m+i++), square, dest);
            if (sq[dest] != ' ' && ENEMIES(sq, square, dest)) break;
        }
    }
    return i;
}


static int king_castles(Piece *sq, int square, Move *m)
{
    static Piece castling_king[] = {BLACK_CASTLING_KING, WHITE_CASTLING_KING};
    static Piece castling_rook[] = {BLACK_CASTLING_ROOK, WHITE_CASTLING_ROOK};
    static Piece moved_king[] = {BLACK_KING, WHITE_KING};
    int piece_index = ISWHITE(sq[square]);
    int i = 0;
    int ks = ISBLACK(sq[square]) ? 4 : 60;

    if (!(square == ks && sq[square] != castling_king[piece_index]))
        return 0;

    if (sq[square+3] == castling_rook[piece_index] && !sq[square+1] && !sq[square+2]) {
        move_with_side_effect((m+i++), square, square+2, KS_CASTLE);
    }
    if (sq[square-4] == castling_rook[piece_index] && !sq[square-1] && !sq[square-2] && !sq[square-3]) {
        move_with_side_effect((m+i++), square, square-2, QS_CASTLE);
    }

    return i;
}
