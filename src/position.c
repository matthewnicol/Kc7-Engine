MoveSet valid_moves(Board *b) /* List all valid moves on a board */
{
    int i;
    Move **moves = malloc(sizeof(Move*)*60);
    MoveSet mset = {moves, 0};
    Board *bscratch = copy_board(b);
    for (i = 0; i < 64; i++) {
        mset.count += get_piece_moves(b->piecemap, i, b->turn, (mset.moves + mset.count));
    }

    mset = trim_invalid_moves(bscratch, mset);
    free(bscratch);
    return mset;

//    if (move_count > 0) {
//        printf("Available Moves: \n");
//        for (i = 0; i < move_count; i++) {
//            reprMove(*(moves + i));
//            if (i % 5 == 0) printf("\n");
//        }
//        printf("\n\n");
//        i = rand() % move_count;
//        applyMove(b, *(moves+i));
//        printf("My choice: ");
//        reprMove(*(moves+i));
//        printf("\n\n-------------------------------------------------\n");
//        sleep(2);
//    };
//    free(moves);
//    b->turn = b->turn == PLAYER_WHITE ? PLAYER_BLACK : PLAYER_WHITE;

} 

int is_checkmate(Board *b, MoveSet mset)
{
    return *(mset.moves+0) == NULL 
        && square_is_attacked(b->piecemap, locate_king(b->piecemap, b->turn), !b->turn);
}

int is_stalemate(Board *b, MoveSet mset)
{
    return *(mset.moves+0) == NULL && !is_checkmate(b, mset);
}

int handle_position(PositionStrategy strat, Board *b) 
{
    if (strat == MAKE_RANDOM_MOVE) {
        MoveSet mset = valid_moves(b);
        int i;
        for (i = 0; i < mset.count; i++) {
            reprMove(*(mset.moves + i));
            if (i % 5 == 0) printf("\n");
        }
        printf("\n\n");
        if (*(mset.moves+0) != NULL) {
            applyMove(b, *(mset.moves+rand()%mset.count));
            b->turn = !b->turn;
            sleep(2);
        } else {
            if (is_checkmate(b, mset)) {
                printf("Checkmate!\n");
            } else if (is_stalemate(b, mset)) {
                printf("Stalemate!\n");
            }
        }
    }
    return 0;
}
