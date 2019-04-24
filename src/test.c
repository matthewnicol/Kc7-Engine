#include "chesseng.h"
void check_pieces(Board*);
void check_squares(void);
void check_moves(Board*);

int main ()
{
    Board *b = new_board();
    assert (b != NULL);
    standard_position(b);
    check_squares();
    check_pieces(b);
    check_moves(b);
}

void check_squares() {
    printf("Checking VALID(...)\n");
    if (VALID(-1)) {
        printf("Square -1 mistakenly marked as a valid square.\n");
    }
    if (VALID(64)) {
        printf("Square 64 mistakenly marked as a valid square.\n");
    }
    printf("Checking FILE_MAP[...]\n");
    if (FILE_MAP[63] != 'h') {
        printf("Square 63 not identified as file h.\n");
    }
    if (FILE_MAP[62] != 'g') {
        printf("Square 62 not identified as file g.\n");
    }
    if (FILE_MAP[61] != 'f') {
        printf("Square 61 not identified as file f.\n");
    }
    if (FILE_MAP[60] != 'e') {
        printf("Square 60 not identified as file e.\n");
    }
    if (FILE_MAP[59] != 'd') {
        printf("Square 59 not identified as file d.\n");
    }
    if (FILE_MAP[58] != 'c') {
        printf("Square 58 not identified as file c.\n");
    }
    if (FILE_MAP[57] != 'b') {
        printf("Square 57 not identified as file b.\n");
    }
    if (FILE_MAP[56] != 'a') {
        printf("Square 56 not identified as file a.\n");
    }

}

void check_pieces(Board *b) {
    printf("Checking piece equality\n");
    if (b->squares[0] != BLACK_CASTLING_ROOK) {
        printf("Did not detect black castling rook on square 0 after standard_position...\n");
    }
    if (b->squares[1] != BLACK_KNIGHT) {
        printf("Did not detect black knight on square 1 after standard_position...\n");
    }
    if (b->squares[2] != BLACK_BISHOP) {
        printf("Did not detect black bishop on square 2 after standard_position...\n");
    }
    if (b->squares[3] != BLACK_QUEEN) {
        printf("Did not detect black queen on square 3 after standard_position...\n");
    }
    if (b->squares[4] != BLACK_CASTLING_KING) {
        printf("Did not detect black castling king on square 4 after standard_position...\n");
    }
    if (b->squares[8] != BLACK_PAWN) {
        printf("Did not detect black pawn on square 8 after standard_position...\n");
    }

    printf("Checking ENEMIES(...)\n");
    if (ENEMIES(b->squares, 0, 1)) {
        printf("BLACK_CASTLING_ROOK && BLACK KNIGHT incorrectly identified as opponents.\n");
    }
    if (!ENEMIES(b->squares, 0, 63)) {
        printf("BLACK_CASTLING_ROOK && WHITE_CASTLING_ROOK incorrectly not identified as opponents.\n");
    }
    if (ENEMIES(b->squares, 0, 33)) {
        printf("BLACK_CASTLING_ROOK && NO_PIECE incorrectly identified as opponents.\n");
    }
    if (ENEMIES(b->squares, 61, 30)) {
        printf("WHITE_QUEEN && NO_PIECE incorrectly identified as opponents.\n");
    }

    printf("Checking FRIENDS(...)\n");
    if (!FRIENDS(b->squares, 0, 1)) {
        printf("BLACK_CASTLING_ROOK && BLACK KNIGHT incorrectly identified as not friends.\n");
    }
    if (FRIENDS(b->squares, 0, 63)) {
        printf("BLACK_CASTLING_ROOK && WHITE_CASTLING_ROOK incorrectly not identified as friends.\n");
    }
    if (FRIENDS(b->squares, 0, 33)) {
        printf("BLACK_CASTLING_ROOK && NO_PIECE incorrectly identified as friends.\n");
    }
    if (FRIENDS(b->squares, 61, 30)) {
        printf("WHITE_QUEEN && NO_PIECE incorrectly identified as friends.\n");
    }
}

void check_moves(Board *b)
{
    Move m = {52, 36, WHITE_PAWN, NO_PIECE};
    Move m2 = {13, 29, BLACK_PAWN, NO_PIECE};
    Move m3 = {51, 35, WHITE_PAWN, NO_PIECE};

    printf("Checking apply_move(...)\n");
    apply_move(b->squares, &m);
    
    if (b->squares[36] != WHITE_EP_PAWN) {
        printf("White EP Pawn not found on square 36 after making a move.\n");
    }

    apply_move(b->squares, &m2);

    if (b->squares[36] == WHITE_EP_PAWN) {
        printf("White EP Pawn not removed after subsequent move.\n");
    }

    apply_move(b->squares, &m3);

    if (b->squares[29] == BLACK_EP_PAWN) {
        printf("Black EP Pawn not removed after subsequent move.\n");
    }
    printBoard(b->squares);

    printf("Checking moves_for_square(...)\n");

    //6
    Move *pm = malloc(sizeof(Move)*20);
    int mcount = moves_for_square(b->squares, 60, WHITE, pm);
    if (mcount != 2) {
        printf("Tried generating moves for king. Expected 2... but got %i\n", mcount);
    }
    mcount = moves_for_square(b->squares, 59, WHITE, pm);
    if (mcount != 6) {
        printf("Tried generating moves for queen. Expected 6... but got %i\n", mcount);
    }

    mcount = moves_for_square(b->squares, 29, BLACK, pm);
    if (mcount != 2) {
        printf("Tried generating moves for pawn. Expected 2... but got %i\n", mcount);
    }
    if (pm[0].to != 36 && pm[1].to != 36) {
        printf("Checking that one of the pawn moves is a capture... it isn't\n");
    }

    mcount = moves_for_square(b->squares, 62, WHITE, pm);
    if (mcount != 3) {
        printf("Tried generating moves for knight. Expected 3... but got %i\n", mcount);
    }


    
}
