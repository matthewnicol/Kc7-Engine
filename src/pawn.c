int pawn_moves(Board *b, Move **moves, int square) 
{
    int i = 0;
    int twosquares, blockaded2, blockaded1, ep_left, ep_right;
    if (is_white[b->piecemap[square]]) {
        twosquares = RANK_MAP[square] == 2;
        blockaded1 = OCCUPIED(b, square-8);
        blockaded2 = blockaded1 && OCCUPIED(b, square-16);
        ep_left = ONSQ(b, square-1, BLACK_EP_PAWN) && SAMERANK(square, square-1);
        ep_right = ONSQ(b, square+1, BLACK_EP_PAWN) && SAMERANK(square, square+1);
    } else {
        twosquares = RANK_MAP[square] == 7;
        blockaded1 = !OCCUPIED(b, square+8);
        blockaded2 = blockaded1 && !OCCUPIED(b, square+16);
        ep_left = ONSQ(b, square-1, WHITE_EP_PAWN) && SAMERANK(square, square-1);
        ep_right = ONSQ(b, square+1, WHITE_EP_PAWN) && SAMERANK(square, square+1);
    }
    if (!blockaded1) {
        if (is_black[b->piecemap[square]] && VALID(square+8)) 
            moves[i++] = makeMove( makePieceMovement(square, square+8, BLACK_PAWN, NO_PIECE), blankPieceMovement()); 
        else if (is_white[b->piecemap[square]] && VALID(square-8))
            moves[i++] = makeMove( makePieceMovement(square, square-8, WHITE_PAWN, NO_PIECE), blankPieceMovement());
        if (!blockaded2 && twosquares) {
            if (is_black[b->piecemap[square]] && VALID(square+16))
                moves[i++] = makeMove( makePieceMovement(square, square+16, BLACK_PAWN, NO_PIECE), blankPieceMovement());
             else if (is_white[b->piecemap[square]] && VALID(square-16))
                moves[i++] = makeMove( makePieceMovement(square, square-16, WHITE_PAWN, NO_PIECE), blankPieceMovement());
        }
    }
    if (ep_right) {
        if (is_black[b->piecemap[square]] && VALID(square+9))
            moves[i++] = makeMove(
                    makePieceMovement(square, square+9, WHITE_PAWN, NO_PIECE),
                    makePieceMovement(square, square+1, NO_PIECE, WHITE_PAWN));
        else if (is_white[b->piecemap[square]] && VALID(square-9))
            moves[i++] = makeMove(
                    makePieceMovement(square, square-9, BLACK_PAWN, NO_PIECE),
                    makePieceMovement(square, square-1, NO_PIECE, BLACK_PAWN));
    }
    if (ep_left) {
        if (is_black[b->piecemap[square]] && VALID(square+7)) {
            moves[i++] = makeMove(
                    makePieceMovement(square, square+7, WHITE_PAWN, NO_PIECE),
                    makePieceMovement(square, square-1, NO_PIECE, WHITE_PAWN));
        }
        else if (is_white[b->piecemap[square]] && VALID(square-7)) {
            moves[i++] = makeMove(
                    makePieceMovement(square, square-7, BLACK_PAWN, NO_PIECE),
                    makePieceMovement(square, square+1, NO_PIECE, BLACK_PAWN));
        }
    }
    return i;
}
