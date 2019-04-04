int pawn_moves(Piece piecemap[], int square, Player turn, Move **moves) 
{
    int i = 0;
    int direction = turn == PLAYER_BLACK ? 1 : -1;
    Piece ep_target = turn == PLAYER_BLACK? WHITE_EP_PAWN : BLACK_EP_PAWN;

    int  ep_left, ep_right;

    ep_left = piecemap[square+(-1*direction)] == ep_target && RANK_MAP[square] == RANK_MAP[square+(-1*direction)];
    ep_right = piecemap[square+(1*direction)] == ep_target && RANK_MAP[square] == RANK_MAP[square+(1*direction)];

    // One step moves
    if (!piecemap[square+(8*direction)] && VALID(square+(8*direction))) 
        makeSimpleMove(moves[i++], square, square+(8*direction), piecemap[square], NO_PIECE);

    // Two step moves
    if (i && !piecemap[square+(16*direction)] && RANK_MAP[square] == (turn ? 7 : 2))
        makeSimpleMove(moves[i++], square, square+(16*direction), piecemap[square], NO_PIECE); 

    // Diagonal Capture
    if (VALID(square+(9*direction)) && different_team(piecemap, square, square+(9*direction)))
        makeSimpleMove(moves[i++], square, square+(9*direction), piecemap[square], piecemap[square+(9*direction)]); 
    
    // Diagonal Capture
    if (VALID(square+(7*direction)) && different_team(piecemap, square, square+(7*direction)))
        makeSimpleMove(moves[i++], square, square+(7*direction), piecemap[square], piecemap[square+(7*direction)]); 


    // En Passant to the Left
    if (ep_right && VALID(square+(9*direction))) {
            makeSimpleMove(moves[i++], square, square+(9*direction), piecemap[square], NO_PIECE);
            addComplexMovement(moves[i-1], square, square+(1*direction), NO_PIECE, ep_target);
    }

    // En Passant to the Right
    if (ep_left && VALID(square+(7*direction))) {
            makeSimpleMove(moves[i++], square, square+(7*direction), piecemap[square], NO_PIECE);
            addComplexMovement(moves[i-1], -1, square-(1*direction), NO_PIECE, ep_target);
    }
    return i;
}
