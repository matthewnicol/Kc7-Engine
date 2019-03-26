PieceMovement* blankPieceMovement ()
{
    return NULL;
}

PieceMovement* makePieceMovement (int from, int to, Piece on_from, Piece on_to)
{
    PieceMovement *p = malloc(sizeof(PieceMovement));
    (*p).from = from;
    (*p).to = to;
    (*p).on_from = on_from;
    (*p).on_to = on_to;
    return p;
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

