PieceMovement* blankPieceMovement ()
{
    return NULL;
}

PieceMovement* makePieceMovement (int from, int to, Piece on_from, Piece on_to)
{
    assert(from >= 0 && from <= 63);
    assert(to >= 0 && to <= 63);
    assert(on_from < 22);
    assert(on_to < 22);
    PieceMovement *p = malloc(sizeof(PieceMovement));
    (*p).from = from;
    (*p).to = to;
    (*p).on_from = on_from;
    (*p).on_to = on_to;
    return p;
}

Move *makeSimpleMove(int from, int to, Piece on_from, Piece on_to)
{
    Move *m = malloc(sizeof(Move));
    m->main = malloc(sizeof(PieceMovement));
    assert(from >= 0 && from <= 63);
    assert(to >= 0 && to <= 63);
    assert(on_from < 22);
    assert(on_to < 22);
    (*(m->main)).from = from;
    (*(m->main)).to = to;
    (*(m->main)).on_from = on_from;
    (*(m->main)).on_to = on_to;
    m->alt = NULL;
    return m;
}

Move *makeMove (PieceMovement *main, PieceMovement *alt)
{
    Move *m = malloc(sizeof(Move));
    m->main = main;
    m->alt = alt;
    return m;
}


void applyMove(Board *b, Move *m)
{
    b->piecemap[m->main->to] = m->main->on_from;
    b->piecemap[m->main->from] = NO_PIECE;

    if (m->alt != NULL) {
        b->piecemap[m->alt->to] = m->alt->on_from;
        if (m->alt->from >= 0 && m->alt->from < 64) {
            b->piecemap[m->alt->from] = NO_PIECE;
        }
    }
}

void reverseMove(Board *b, Move *m)
{
    b->piecemap[m->main->from] = m->main->on_from;
    b->piecemap[m->main->to] = m->main->on_to;
    if (m->alt != NULL) {
        b->piecemap[m->alt->to] = m->alt->on_to;
        b->piecemap[m->alt->from] = m->alt->on_from;
    }
}

char* move_to_algebraic(Move *m) {
    char *moverepr = malloc(sizeof(char)*7);
    memset(moverepr, '\0', 7);
    if (is_king[m->main->on_from]) {
        if ((m->main->from == 60 && m->main->to == 62) || (m->main->from == 4 && m->main->to == 6)) { 
            return "0-0";
        }
        if ((m->main->from == 60 && m->main->to == 58) || (m->main->from == 4 && m->main->to == 2)) { 
            return "0-0-0";
        }
    }

    if (is_pawn[m->main->on_from]) {
        if (FILE_MAP[m->main->to] != FILE_MAP[m->main->from]) {
            sprintf(moverepr, "%cx%c%i", FILE_MAP[m->main->from], FILE_MAP[m->main->to], RANK_MAP[m->main->to]); 
        } else sprintf(moverepr, "%c%i", FILE_MAP[m->main->on_from], RANK_MAP[m->main->to]);
        return moverepr;
    }
    sprintf(
        moverepr, 
        "%c%s%c%i", 
        PIECE_MAP[m->main->on_from], 
        PIECE_MAP[m->main->on_to]? "x" : "",
        FILE_MAP[m->main->to],
        RANK_MAP[m->main->to]
    );
    return moverepr;
}
