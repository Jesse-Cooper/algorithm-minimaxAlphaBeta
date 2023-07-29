#include "minimax.h"

#include <assert.h>


// base scores of each end state
static const int SCORE_WIN = INT8_MAX;
static const int SCORE_LOSE = INT8_MIN;
static const int SCORE_DRAW = 0;


static int8_t minimise(board_t *board,
                       char     symbolSelf,
                       char     symbolOther,
                       uint8_t  depth,
                       int8_t   alpha,
                       int8_t   beta);
static int8_t maximise(board_t *board,
                       char     symbolSelf,
                       char     symbolOther,
                       uint8_t  depth,
                       int8_t   alpha,
                       int8_t   beta);

static int8_t min(int8_t a,
                  int8_t b);
static int8_t max(int8_t a,
                  int8_t b);


/* ------------------------------ START PUBLIC ------------------------------ */


/*
@context
    * Finds the optimal move to make with `symbolSelf` in current `board` state.
    * Uses minimax with alpha-beta pruning.
        * Assuming `board` is 3x3, using this method for an entire game will
          only result in a win or draw for the AI (cannot lose).
        * Depth is used to encouraged to win using the least amount of moves.

@parameters
    * board
        * Current state of the Noughts and Crosses game.
    * symbolSelf
        * Symbol to find best move for.

@return
    * Cell of the optimal move.
*/
uint8_t getBestMove(board_t *board,
                    char     symbolSelf)
{
    uint8_t cell, bestMove;
    int8_t score, alpha, beta;
    char symbolOther;

    // get the symbol of the opponent
    symbolOther = symbolSelf == NOUGHT ? CROSS : NOUGHT;

    alpha = SCORE_LOSE;
    beta = SCORE_WIN;

    // try and score every valid `symbolSelf` move to find next best move
    for (cell = 0; cell < getSize(board) * getSize(board); cell += 1)
    {
        if (isValidMove(board, cell, symbolSelf))
        {
            // make `symbolSelf` move, `score` it and unmake the move
            setCell(board, cell, symbolSelf);
            score = minimise(board, symbolSelf, symbolOther, 1, alpha, beta);
            setCell(board, cell, EMPTY);

            // current `cell` is a better move than the previous best move
            if (score > alpha)
            {
                alpha = score;
                bestMove = cell;
            }
        }
    }

    // no best move found - `board` was full - no move possible
    assert(alpha != SCORE_LOSE);

    return bestMove;
}


/* ------------------------------- END PUBLIC ------------------------------- */
/* ----------------------------- START  PRIVATE ----------------------------- */


/*
@context
    * Simulates the opponent's (`symbolOther`) turn in current `board` state.
    * Assumes opponent is optimal - minimise self score (maximise their own).

@parameters
    * board
        * Current state of the Noughts and Crosses game.
    * symbolSelf
        * Symbol to find best move for.
    * symbolOther
        * Symbol of opponent.
        * Placed this turn.
    * depth
        * Current depth (moves made) of minimax search tree.
    * alpha
        * Highest score of this branch so far.
        * Finding a score lower than this allows this branch to be pruned.
    * beta
        * Lowest score of this branch so far.

@return
    * Score of the best self move at `board` state.
        * Opponent minimise the score, so this is the lowest of score this turn.
        * Provides the score of the move leading to current `board` state.
*/
static int8_t minimise(board_t *board,
                       char     symbolSelf,
                       char     symbolOther,
                       uint8_t  depth,
                       int8_t   alpha,
                       int8_t   beta)
{
    int8_t score;
    uint8_t cell;

    // score `board` end state if reached - win not possible when minimising
    if (isWin(board, symbolOther))
    {
        // penalising `depth` encourages losses using more moves
        return SCORE_LOSE + depth;
    }
    else if (isDraw(board))
    {
        return SCORE_DRAW;
    }

    // try and score every valid `symbolOther` move
    for (cell = 0; cell < getSize(board) * getSize(board); cell += 1)
    {
        if (isValidMove(board, cell, symbolOther))
        {
            // make `symbolOther` move, `score` it and unmake the move
            setCell(board, cell, symbolOther);
            score = maximise(board,
                             symbolSelf,
                             symbolOther,
                             depth + 1,
                             alpha,
                             beta);
            setCell(board, cell, EMPTY);

            // opponent wants to minimise the self `score`
            beta = min(beta, score);

            // prune this branch of minimax if it cannot have a better score
            if (beta <= alpha)
            {
                return alpha;
            }
        }
    }

    return beta;
}


/*
@context
    * Simulates the self (`symbolSelf`) turn in current `board` state.
    * Plays optimally - maximise self score (minimise opponent score).

@parameters
    * board
        * Current state of the Noughts and Crosses game.
    * symbolSelf
        * Symbol to find best move for.
        * Placed in this turn.
    * symbolOther
        * Symbol of opponent.
    * depth
        * Current depth (moves made) of minimax search tree.
    * alpha
        * Highest score of this branch so far.
    * beta
        * Lowest score of this branch so far.
        * Finding a score higher than this allows this branch to be pruned.

@return
    * Score of the best self move at `board` state.
        * Maximises the score, so this is the highest of score this turn.
        * Provides the score of the move leading to current `board` state.
*/
static int8_t maximise(board_t *board,
                       char     symbolSelf,
                       char     symbolOther,
                       uint8_t  depth,
                       int8_t   alpha,
                       int8_t   beta)
{
    int8_t score;
    uint8_t cell;

    // score `board` end state if reached - loss not posible when maximising
    if (isWin(board, symbolSelf))
    {
        // penalising `depth` encourages wins using less moves
        return SCORE_WIN - depth;
    }
    else if (isDraw(board))
    {
        return SCORE_DRAW;
    }

    // try and score every valid `symbolSelf` move
    for (cell = 0; cell < getSize(board) * getSize(board); cell += 1)
    {
        if (isValidMove(board, cell, symbolSelf))
        {
            // make `symbolSelf` move, `score` it and unmake the move
            setCell(board, cell, symbolSelf);
            score = minimise(board,
                             symbolSelf,
                             symbolOther,
                             depth + 1,
                             alpha,
                             beta);
            setCell(board, cell, EMPTY);

            // want to maximise the self `score`
            alpha = max(alpha, score);

            // prune this branch of minimax if it cannot have a smaller score
            if (alpha >= beta)
            {
                return beta;
            }
        }
    }

    return alpha;
}


/*
@context
    * Gets the minimum value between `a` and `b`.

@parameters
    * a
        * Value to return if smaller than `b`.
    * b
        * Value to return if smaller than `a`.

@return
    * Minimum value between `a` and `b`.
*/
static int8_t min(int8_t a,
                  int8_t b)
{
    if (a < b)
    {
        return a;
    }
    return b;
}


/*
@context
    * Gets the maximum value between `a` and `b`.

@parameters
    * a
        * Value to return if larger than `b`.
    * b
        * Value to return if larger than `a`.

@return
    * Maximum value between `a` and `b`.
*/
static int8_t max(int8_t a,
                  int8_t b)
{
    if (a > b)
    {
        return a;
    }
    return b;
}


/* ------------------------------ END  PRIVATE ------------------------------ */
