int pawn_moves(Piece piecemap[64], int square, Player turn, Move **moves) 
{
    int i = 0;
    int direction = turn == PLAYER_BLACK ? 1 : -1;
    Piece ep_target = turn == PLAYER_BLACK? WHITE_EP_PAWN : BLACK_EP_PAWN;

    int  ep_left, ep_right;

    ep_left = piecemap[square+(-1*direction)] == ep_target && RANK_MAP[square] == RANK_MAP[square+(-1*direction)];
    ep_right = piecemap[square+(1*direction)] == ep_target && RANK_MAP[square] == RANK_MAP[square+(1*direction)];

    // One step moves
    if (!piecemap[square+(8*direction)] && VALID(square+(8*direction))) 
        moves[i++] = makeSimpleMove(square, square+(8*direction), piecemap[square], NO_PIECE);

    // Two step moves
    if (i && !piecemap[square+(16*direction)] && RANK_MAP[square] == (turn ? 7 : 2))
        moves[i++] = makeSimpleMove(square, square+(16*direction), piecemap[square], NO_PIECE); 

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
