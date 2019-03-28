int lateral_mover(Piece piecemap[64], int square, Player turn, int updown, int diagonal, Move **moves) 
{
    int i, j, k = 0;
    int blockaded[] = {0, 0, 0, 0, 0, 0, 0, 0};
    char *files[8];
    for (i = 0; i < 8; files[i++] = FILE_MAP[square]);
    
    for (i = 1; i < 9; i++) {
        int directions[] = {
            //Diagonals
            square + (8*i) - i, square + (8*i) + i, square - (8*i) - i, square - (8*i) + i,
            //Up/down/left/right
            square + (8*i), square + (8*i), square - (8*i), square - (8*i)
        };
        for (j = 0; j < 8; j++) {
            int to = directions[j];
            // Bishops & Rooks don't need to process all of these directions
            if ((!updown && j > 3) || (!diagonal && j < 4) || blockaded[j] || !VALID(directions[j])) continue;  

            // Blockaded by the edge of the board - don't jump off
            if ((blockaded[j] = (files[j] == "h" && FILE_MAP[to] == "a") || (files[j] == "a" && FILE_MAP[to] == "h"))) continue; 

            files[j] = FILE_MAP[to];
            if ((blockaded[j] = piecemap[to]) && same_team(piecemap, square, to)) continue;
            moves[k++] = makeSimpleMove(square, to, piecemap[square], piecemap[to]);
        }
    }
    return k;
}
