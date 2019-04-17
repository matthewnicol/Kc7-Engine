static int pawn_advances(Piece *sq, int square, Turn t, Move *m)
{
    int i = 0;
    int direction = t == PLAYER_BLACK? 1 : -1;
    int on_home_rank = COLOURCOND(RANK_MAP[square], t, 2, 7);

    int dst[] = {square+(8*direction), square+(16*direction)};
    for (int k = 0; k < 2; k++) {
        if (!VALID(dst[k]) || sq[dst[k]] || (k == 1 && !on_home_rank)) { return i; }
        basic_move((m+i++), square, dst[k], sq[square], sq[dst[k]]);
    }
    return i;
}

static int pawn_captures(Piece *sq, int square, Turn t, Move *m)
{
    int i = 0;
    int direction = t == PLAYER_WHITE ? -1 : 1;
    int differentials[] = {7, 9};
    for (int k = 0; k < 2; k++) {
        int dst = square+(differentials[k]*direction);
        if (VALID(dst) && OPPONENTS(sq, square, dst)) {
            if (EITHER(FILE_MAP[square], 'a', 'h') && EITHER(FILE_MAP[dst], 'a', 'h')) continue;
            basic_move((m + i++), square, dst, sq[square], sq[dst]);
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
        if (EITHER(FILE_MAP[square], 'a', 'h') && EITHER(FILE_MAP[epsq[k]], 'a', 'h')) continue;
        if (VALID(differentials[k]) && is_black[sq[square]] ? sq[epsq[k]] == WHITE_EP_PAWN : sq[epsq[k]] == BLACK_EP_PAWN) {
            move_with_side_effect((m + i++), square, differentials[k], sq[square], sq[differentials[k]], EP_CAPTURE); 
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

static int knight_moves(Piece *sq, int square, Move *m)
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

static int king_moves(Piece *sq, int square, Move *m)
{
    static int king_diffs[][2] = {
        {0, 1}, {0, -1}, {1, 1}, {1, -1}, {1, 0}, {-1, 1}, {-1, -1}, {-1, 0}
    };

    int i = 0, j;

    for (j = 0; j < 8; j++) {
        int sqto = square + king_diffs[j][0]*8 + king_diffs[j][1];
        if (VALID(sqto) && (!sq[sqto] || different_team(sq, square, sqto))) {
            if (FILE_MAP[sqto] == 'h' && FILE_MAP[square] == 'a') continue;
            if (FILE_MAP[sqto] == 'a' && FILE_MAP[square] == 'h') continue;
            basic_move((m+i++), square, sqto, sq[square], sq[sqto]);
        }
    }
    return i;
}

static int linewise_piece_moves(Piece *sq, int square, int diagonal, int upanddown, Move *m)
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
                basic_move((m+i), square, directions[j], sq[square], sq[directions[j]]);
                i++;
            }
        }
    }
    return i;
}


static int king_castles(Piece *sq, int square, Move *m)
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
