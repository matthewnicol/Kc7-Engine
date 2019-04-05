void addComplexMovement(Move *m, int from, int to, Piece on_from, Piece on_to)
{
    m->s_effect_from = from;
    m->s_effect_to = to;
    m->s_effect_on_from = on_from;
    m->s_effect_on_to = on_to;
}

void makeSimpleMove(Move *m, int from, int to, Piece on_from, Piece on_to)
{
    m->from = from;
    m->to = to;
    m->on_from = on_from;
    m->on_to = on_to;
    m->s_effect_from = -1;
    m->s_effect_to = -1;
    m->s_effect_on_from = NO_PIECE;
    m->s_effect_on_to = NO_PIECE;
}

void applyMove(Board *b, Move *m)
{
    b->piecemap[m->to] = m->on_from;
    b->piecemap[m->from] = NO_PIECE;

    if (m->s_effect_from == -1 && m->s_effect_to == -1 && m->s_effect_on_from == NO_PIECE && m->s_effect_to == NO_PIECE) return;
    b->piecemap[m->s_effect_to] = m->s_effect_on_from;
    if (m->s_effect_from >= 0 && m->s_effect_from < 64) {
        b->piecemap[m->s_effect_from] = NO_PIECE;
    }
}

void reverseMove(Board *b, Move *m)
{
    b->piecemap[m->from] = m->on_from;
    b->piecemap[m->to] = m->on_to;
    if (m->s_effect_from == -1 && m->s_effect_to == -1 && m->s_effect_from == NO_PIECE && m->s_effect_to == NO_PIECE) return;
    b->piecemap[m->s_effect_to] = m->s_effect_on_to;
    b->piecemap[m->s_effect_to] = m->s_effect_on_from;
}

int move_to_algebraic(char *moverepr, Move *m) {
    if (is_king[m->on_from]) {
        if ((m->from == 60 && m->to == 62) || (m->from == 4 && m->to == 6)) { 
            return snprintf(moverepr, sizeof(char)*8, "%s", "0-0");
        }
        if ((m->from == 60 && m->to == 58) || (m->from == 4 && m->to == 2)) { 
            return snprintf(moverepr, sizeof(char)*8, "%s", "0-0-0");
        }
    }

    if (is_pawn[m->on_from]) {
        if (FILE_MAP[m->to] != FILE_MAP[m->from]) {
            return snprintf(
                moverepr, sizeof(char)*8, 
                "%cx%c%i", FILE_MAP[m->from], FILE_MAP[m->to], RANK_MAP[m->to]
             ); 
        } else return snprintf(moverepr, sizeof(char)*8, "%c%i", FILE_MAP[m->on_from], RANK_MAP[m->to]);
    }
    return snprintf(
        moverepr, 
        sizeof(char)*8,
        "%c%s%c%i", 
        PIECE_MAP[m->on_from], 
        PIECE_MAP[m->on_to]? "x" : "",
        FILE_MAP[m->to],
        RANK_MAP[m->to]
    );
}
