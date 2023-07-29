/*
@context
    * Provides a method to find the optimal move of a Noughts and Crosses state.
    * Uses minimax with alpha-beta pruning.
        * Assuming `board` is 3x3, using this method for an entire game will
        only result in a win or draw for the AI (cannot lose).
        * Depth is used to encouraged to win using the least amount of moves.
*/


#ifndef _MINIMAX_H
    #define _MINIMAX_H

    #include <stdint.h>

    #include "board.h"


    uint8_t getBestMove(board_t *board,
                        char     symbolSelf);

#endif
