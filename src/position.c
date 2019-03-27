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
        printf("Available Moves: \n");
        int s = 0;
        for (i = 0; i < move_count; i++) {
            if (++s == 5) { printf("\n"); s = 0;}
            reprMove(*(moves + i));
        }
        printf("\n\n");
        i = rand() % move_count;
        applyMove(b, *(moves+i));
        printf("My choice: ");
        reprMove(*(moves+i));
        printf("\n\n-------------------------------------------------\n");
    };
    free(moves);
    b->turn = b->turn == PLAYER_WHITE ? PLAYER_BLACK : PLAYER_WHITE;

} 

/* If there are no valid moves, and the king is attacked, then it is checkmate */
int is_checkmate(Board *b)
{
}
