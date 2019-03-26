
int get_piece_moves(Piece piecemap[64], int square, Player turn, Move **moves) 
{
    if (piecemap[square] == NO_PIECE) return 0;
    if (turn == PLAYER_WHITE && is_black[piecemap[square]]) return 0;
    if (turn == PLAYER_BLACK && is_white[piecemap[square]]) return 0;

    if (PIECE_MAP[piecemap[square]] == "P") return pawn_moves(piecemap, square, turn, moves);
    if (PIECE_MAP[piecemap[square]] == "N") return knight_moves(piecemap, square, turn, moves);
    if (PIECE_MAP[piecemap[square]] == "B") return bishop_moves(piecemap, square, turn, moves);
    if (PIECE_MAP[piecemap[square]] == "R") return rook_moves(piecemap, square, turn, moves);
    if (PIECE_MAP[piecemap[square]] == "Q") return queen_moves(piecemap, square, turn, moves);
    if (PIECE_MAP[piecemap[square]] == "K") return king_moves(piecemap, square, turn, moves);
}

void validMoves(Board *b) /* List all valid moves on a current board */
{
    int move_count = 0;
    int i;
    Move **moves = malloc(sizeof(Move*)*60);
    Board *bscratch = copy_board(b);
    for (i = 0; i < 64; i++) {
        move_count += get_piece_moves(b->piecemap, i, b->turn, (moves + move_count));
    }

    move_count = trim_invalid_positions(bscratch, moves, move_count);
    free(bscratch);
    
    printf("\nTotal Moves: %i\n", move_count);

    i = rand() % move_count;
    applyMove(b, *(moves+i));
    reprMove(*(moves+i));
    printf("\n");
    free(moves);
    b->turn = b->turn == PLAYER_WHITE ? PLAYER_BLACK : PLAYER_WHITE;

} 


int can_opponent_attack(Board *b, int square) {
    int move_count = 0;
    int i, j;
    Move **move_holder = malloc(sizeof(Move*)*20);
    for (i = 0; i < 64; i++) {
        move_count = get_piece_moves(b->piecemap, i, b->turn, move_holder);
        for (j = 0; j < move_count; j++) {
            Move *m = *(move_holder + j);
            if (m->main->to == square) {
                free(move_holder);
                return 1;
            }
        }
    }
    free(move_holder);
    return 0;
}
        

int trim_invalid_positions(Board *b, Move **m, int candidates)
{
    int i, sq, kingsq, k = 0;
    b->turn = b->turn == PLAYER_WHITE? PLAYER_BLACK : PLAYER_WHITE;

    for (i = 0; i < candidates; i++) {
        applyMove(b, *(m+i));
        for (sq = 0; sq < 64; sq++) {
            if ((b->piecemap[sq] == WHITE_KING && b->turn == PLAYER_WHITE) || (b->piecemap[sq] == BLACK_KING && b->turn == PLAYER_BLACK)) {
                if (!can_opponent_attack(b, sq)) *(m + k++) = *(m + i);
                break;
            }
        }
        reverseMove(b, *(m+i));
    }
    return k;
}
