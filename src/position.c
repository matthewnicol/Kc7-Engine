void valid_moves(Board *b) /* List all valid moves on a board */
{
    int i, move_count = 0;
    Move **moves = malloc(sizeof(Move*)*60);
    Board *bscratch = copy_board(b);
    for (i = 0; i < 64; i++) {
        move_count += get_piece_moves(b->piecemap, i, b->turn, (moves + move_count));
    }

    move_count = trim_invalid_moves(bscratch, moves, move_count);
    free(bscratch);
    
    printf("\nTotal Moves: %i\n", move_count);

    if (move_count > 0) {
        i = rand() % move_count;
        applyMove(b, *(moves+i));
        reprMove(*(moves+i));
        printf("\n");
    };
    free(moves);
    b->turn = b->turn == PLAYER_WHITE ? PLAYER_BLACK : PLAYER_WHITE;

} 

/* If there are no valid moves, and the king is attacked, then it is checkmate */
int is_checkmate(Board *b)
{
}
