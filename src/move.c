void makePieceMovement (PieceMovement *p, int from, int to, Piece on_from, Piece on_to)
{
    assert(from >= 0 && from <= 63);
    assert(to >= 0 && to <= 63);
    assert(on_from < 22);
    assert(on_to < 22);
    (*p).from = from;
    (*p).to = to;
    (*p).on_from = on_from;
    (*p).on_to = on_to;
}

void addComplexMovement(Move *m, int from, int to, Piece on_from, Piece on_to)
{
    makePieceMovement(&m->alt, from, to, on_from, on_to);
}

void makeSimpleMove(Move *m, int from, int to, Piece on_from, Piece on_to)
{
    makePieceMovement(m->main, from, to, on_from, on_to);
    makePieceMovement(m->alt, -1, -1, NO_PIECE, NO_PIECE);
}

void makeMove (Move *m, PieceMovement *main, PieceMovement *alt)
{
    m->main = main;
    m->alt = alt;
}


void applyMove(Board *b, Move *m)
{
    b->piecemap[m->main.to] = m->main.on_from;
    b->piecemap[m->main.from] = NO_PIECE;

    if (m->alt.from == -1 && m->alt.to == -1 && m->alt.on_from == NO_PIECE && m->alt.to == NO_PIECE) return;
    b->piecemap[m->alt.to] = m->alt.on_from;
    if (m->alt.from >= 0 && m->alt.from < 64) {
        b->piecemap[m->alt.from] = NO_PIECE;
    }
}

void reverseMove(Board *b, Move *m)
{
    b->piecemap[m->main.from] = m->main.on_from;
    b->piecemap[m->main.to] = m->main.on_to;
    if (m->alt.from == -1 && m->alt.to == -1 && m->alt.on_from == NO_PIECE && m->alt.to == NO_PIECE) return;
    b->piecemap[m->alt.to] = m->alt.on_to;
    b->piecemap[m->alt.from] = m->alt.on_from;
}

char* move_to_algebraic(Move *m) {
    char *moverepr = malloc(sizeof(char)*7);
    memset(moverepr, '\0', 7);
    if (is_king[m->main.on_from]) {
        if ((m->main.from == 60 && m->main.to == 62) || (m->main.from == 4 && m->main.to == 6)) { 
            return "0-0";
        }
        if ((m->main.from == 60 && m->main.to == 58) || (m->main.from == 4 && m->main.to == 2)) { 
            return "0-0-0";
        }
    }

    if (is_pawn[m->main.on_from]) {
        if (FILE_MAP[m->main.to] != FILE_MAP[m->main.from]) {
            sprintf(moverepr, "%cx%c%i", FILE_MAP[m->main.from], FILE_MAP[m->main.to], RANK_MAP[m->main.to]); 
        } else sprintf(moverepr, "%c%i", FILE_MAP[m->main.on_from], RANK_MAP[m->main.to]);
        return moverepr;
    }
    sprintf(
        moverepr, 
        "%c%s%c%i", 
        PIECE_MAP[m->main.on_from], 
        PIECE_MAP[m->main.on_to]? "x" : "",
        FILE_MAP[m->main.to],
        RANK_MAP[m->main.to]
    );
    return moverepr;
}
