#include "chesseng.h"

int main ()
{
    Board b;
    FEN("4rr2/pb1n1p1k/1p2P1pp/2pP4/2P3nq/3B2N1/PB1Q2PP/4RRK1 w - -", &b);
    exit(0);
}

