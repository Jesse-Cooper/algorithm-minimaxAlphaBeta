#include "board.h"

#include <assert.h>
#include <stdlib.h>


struct board_s
{
    uint8_t size;
    char *cells;
};


static bool isWinRow(board_t *board,
                     uint8_t  row,
                     char     symbol);
static bool isWinCol(board_t *board,
                     uint8_t  column,
                     char     symbol);

static bool isWinForwardDiag(board_t *board,
                             char     symbol);
static bool isWinBackwardDiag(board_t *board,
                              char     symbol);


/* ------------------------------ START PUBLIC ------------------------------ */


/*
@context
    * Initialises an empty board.

@parameters
    * size
        * Width and height of the board.
        * The board is always square.

@return
    * Empty board.
*/
board_t *initBoard(uint8_t size)
{
    board_t *board;

    board = malloc(sizeof(board_t));
    assert(board != NULL);

    board->size = size;

    // instead of a 2D array a 1D array is used cells organised row-wise
    board->cells = malloc(sizeof(char) * size);
    assert(board->cells != NULL);

    resetBoard(board);

    return board;
}


/*
@context
    * Frees `board`.

@parameters
    * board
        * Board to free.
*/
void freeBoard(board_t *board)
{
    free(board->cells);
    free(board);
}


/*
@context
    * Resets all cells within `board` back to empty spaces.

@parameters
    * board
        * Board to reset.
*/
void resetBoard(board_t *board)
{
    uint8_t cell;

    for (cell = 0; cell < board->size * board->size; cell += 1)
    {
        board->cells[cell] = EMPTY;
    }
}


/*
@context
    * Determines if a game ends in a win for `symbol`.
    * Wins if `symbol` fills at least 1 row, column or diagonal.

@parameters
    * board
        * Board to determine if ended in a win for `symbol`.
    * symbol
        * User symbol to determine if they have won.

@return
    * Indicates if the game ends in a win for `symbol`.
*/
bool isWin(board_t *board,
           char     symbol)
{
    uint8_t i;

    // check both diagonals for a win
    if (isWinForwardDiag(board, symbol) || isWinBackwardDiag(board, symbol))
    {
        return true;
    }

    // check each row and column
    for (i = 0; i < board->size; i += 1)
    {
        if (isWinRow(board, i, symbol) || isWinCol(board, i, symbol))
        {
            return true;
        }
    }

    return false;
}


/*
@context
    * Determines if a game ends in a draw.
    * Draws occur when there is no win for any symbol and `board` full.

@parameters
    * board
        * Board to determine if ended in a draw.

@return
    * Indicates if the game ends in a draw.
*/
bool isDraw(board_t *board)
{
    uint8_t cell;

    // no draw if there is a win - even if `board` full
    if (isWin(board, NOUGHT) || isWin(board, CROSS))
    {
        return false;
    }

    // check if any cells are empty
    for (cell = 0; cell < board->size * board->size; cell += 1)
    {
        if (board->cells[cell] == EMPTY)
        {
            return false;
        }
    }

    return true;
}


/*
@context
    * Determines if a move in a `cell` is valid within `board`.

@parameters
    * board
        * Board to make move in.
    * cell
        * Position in `board` to make a move.
    * symbol
        * Symbol to place in `cell`.
        * `NOUGHT` and `CROSS` can only be placed on `EMPTY`.
        * `EMPTY` can always be placed.

@return
    * Indicates if move in `cell` is valid within `board`.
*/
bool isValidMove(board_t *board,
                 uint8_t  cell,
                 char     symbol)
{
    return (cell >= 0 && cell < board->size * board->size
        && (symbol == EMPTY || board->cells[cell] == EMPTY));
}


/*
@context
    * Gets the size of `board`.
    * The size is the height and width of `board` (height = width).

@parameters
    * board
        * Board to get the size of.

@return
    * Size of `board`.
*/
uint8_t getSize(board_t *board)
{
    return board->size;
}


/*
@context
    * Gets the symbol at `cell` within `board`.

@parameters
    * board
        * Board to get the `cell` of.
    * cell
        * Position in `board` to get symbol of.

@return
    * Symbol at `cell` within `board`.
*/
char getCell(board_t *board,
             uint8_t  cell)
{
    assert(isValidMove(board, cell, EMPTY));
    return board->cells[cell];
}


/*
@context
    * Sets the `cell` within `board` to `symbol`.
    * Used to make (`NOUGHT` or `CROSS`) or unmake (`EMPTY`) moves.

@parameters
    * board
        * Board to set the `cell` of.
    * cell
        * Position in `board` to set.
        * Must be a valid move.
    * symbol
        * Symbol to set `cell` to.
        * Only `NOUGHT`, `CROSS` or `EMPTY` allowed.
*/
void setCell(board_t *board,
             uint8_t  cell,
             char     symbol)
{
    assert(symbol == NOUGHT || symbol == CROSS || symbol == EMPTY);
    assert(isValidMove(board, cell, symbol));
    board->cells[cell] = symbol;
}


/* ------------------------------- END PUBLIC ------------------------------- */
/* ----------------------------- START  PRIVATE ----------------------------- */


/*
@context
    * Determines if a single `row` ends a game in a win for `symbol`.

@parameters
    * board
        * Board to determine if ended in a win for `symbol`.
    * row
        * Row to check if won in.
        * Range: `0` to `board->size - 1`
    * symbol
        * Symbol to determine if they have won.

@return
    * Indicates if the game ends in a win for `symbol` in `row`.
*/
static bool isWinRow(board_t *board,
                     uint8_t  row,
                     char     symbol)
{
    uint8_t i;

    for (i = 0; i < board->size; i += 1)
    {
        if (board->cells[(board->size * row) + i] != symbol)
        {
            return false;
        }
    }
    return true;
}


/*
@context
    * Determines if a single `column` ends a game in a win for `symbol`.

@parameters
    * board
        * Board to determine if ended in a win for `symbol`.
    * column
        * Column to check if won in.
        * Range: `0` to `board->size - 1`
    * symbol
        * Symbol to determine if they have won.

@return
    * Indicates if the game ends in a win for `symbol` in `column`.
*/
static bool isWinCol(board_t *board,
                     uint8_t  column,
                     char     symbol)
{
    uint8_t i;

    for (i = 0; i < board->size; i += 1)
    {
        if (board->cells[column + (board->size * i)] != symbol)
        {
            return false;
        }
    }
    return true;
}


/*
@context
    * Determines if the forward (\) diagonal ends a game in a win for `symbol`.

@parameters
    * board
        * Board to determine if ended in a win for `symbol`.
    * symbol
        * Symbol to determine if they have won.

@return
    * Indicates if the game ends in a win for `symbol` in the forward diagonal.
*/
static bool isWinForwardDiag(board_t *board,
                             char     symbol)
{
    uint8_t i;

    for (i = 0; i < board->size; i += 1)
    {
        if (board->cells[(board->size * i) + i] != symbol)
        {
            return false;
        }
    }
    return true;
}


/*
@context
    * Determines if the backward (/) diagonal ends a game in a win for `symbol`.

@parameters
    * board
        * Board to determine if ended in a win for `symbol`.
    * symbol
        * Symbol to determine if they have won.

@return
    * Indicates if the game ends in a win for `symbol` in the backward diagonal.
*/
static bool isWinBackwardDiag(board_t *board,
                              char     symbol)
{
    uint8_t i;

    for (i = 0; i < board->size; i += 1)
    {
        if (board->cells[(board->size * (board->size - 1 - i)) + i] != symbol)
        {
            return false;
        }
    }
    return true;
}


/* ------------------------------ END  PRIVATE ------------------------------ */
