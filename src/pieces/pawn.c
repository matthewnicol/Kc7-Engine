int pawn_moves(Piece piecemap[64], int square, Player turn, Move **moves) 
{
    int i = 0;
    int direction = turn == PLAYER_BLACK ? 1 : -1;
    int two_from = turn == PLAYER_BLACK ? 7 : 2;
    Piece ep_target = turn == PLAYER_BLACK? WHITE_EP_PAWN : BLACK_EP_PAWN;

    int twosquares, blockaded2, blockaded1, ep_left, ep_right;

    blockaded1 = piecemap[square+(8*direction)]; 
    twosquares = RANK_MAP[square] == two_from;
    blockaded2 = blockaded1 || piecemap[square+(16*direction)];
    ep_left = piecemap[square+(-1*direction)] == ep_target && RANK_MAP[square] == RANK_MAP[square+(-1*direction)];
    ep_right = piecemap[square+(1*direction)] == ep_target && RANK_MAP[square] == RANK_MAP[square+(1*direction)];

    // One step moves
    if (!blockaded1 && VALID(square+(8*direction))) 
        moves[i++] = makeMove(
                makePieceMovement(square, square+(8*direction), piecemap[square], NO_PIECE), 
                blankPieceMovement()); 


    // Two step moves
    if (!blockaded2 && twosquares)
        moves[i++] = makeMove(
                makePieceMovement(square, square+(16*direction), piecemap[square], NO_PIECE), 
                blankPieceMovement());

    // En Passant to the Left
    if (ep_right && VALID(square+(9*direction))) 
            moves[i++] = makeMove(
                    makePieceMovement(square, square+(9*direction), piecemap[square], NO_PIECE),
                    makePieceMovement(square, square+(1*direction), NO_PIECE, ep_target));

    // En Passant to the Right
    if (ep_left && VALID(square+(7*direction)))
            moves[i++] = makeMove(
                    makePieceMovement(square, square+(7*direction), piecemap[square], NO_PIECE),
                    makePieceMovement(square, square-(1*direction), NO_PIECE, ep_target));
    return i;
}
