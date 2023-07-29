/*
@context
    * Demonstrates the Minimax algorithm with Alpha-Beta pruning.
    * Allows the user to play standard 3x3 Noughts and Crosses games against an
      optimal AI.
        * User can never win only ever draw or lose.
*/


#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "board.h"
#include "interface.h"
#include "minimax.h"


static const char KEY_QUIT = 'q';
static const char KEY_YES = 'y';
static const char KEY_NO = 'n';


static void play(board_t *board);
static bool turn(board_t *board,
                 char     symbolUser,
                 char     symbolAI);

static bool moveUser(board_t *board,
                     char     symbolUser,
                     char     symbolAI);
static bool moveAI(board_t *board,
                   char     symbolUser,
                   char     symbolAI);

static bool isEndState(board_t *board,
                       char     symbolUser,
                       char     symbolAI);

static char getSymbol();
static bool isReplay();

static char getInputLoop(board_t *board,
                         char     symbol,
                         bool     isYesNo,
                         bool     isNumber);


/*
@context
    * Entry point of program.
    * Creates the interface and plays Noughts and Crosses game until user quits.

@return
    * Indicates program successfully terminates.
*/
int main()
{
    board_t *board;

    // initialise interface and play games until user quits
    if (initInterface())
    {
        board = initBoard(BOARD_SIZE);

        play(board);

        freeInterface();
        freeBoard(board);
    }

    return EXIT_SUCCESS;
}


/*
@context
    * Repeatedly plays games until user quits.

@parameters
    * board
        * Noughts and Crosses game state.
*/
static void play(board_t *board)
{
    uint8_t cell;
    char symbolUser, symbolAI;

    // first game skips asking if user want to play a game - just gets order
    if ((symbolUser = getSymbol()) == KEY_QUIT)
    {
        return;
    }

    do
    {
        // clear `board` and interface for a new game
        resetBoard(board);
        for (cell = 0; cell < getSize(board) * getSize(board); cell += 1)
        {
            updateBoard(cell, EMPTY);
        }

        symbolAI = symbolUser == NOUGHT ? CROSS : NOUGHT;

        // play game - turns are made until end state reached or user quits
        updateMessage(MSG_MOVE);
        while (turn(board, symbolUser, symbolAI));
    }
    // ask user if they want to play another game and if so then what order
    while (isReplay() && (symbolUser = getSymbol()) != KEY_QUIT);
}


/*
@context
    * Makes a single turn (user and AI each make 1 move).
    * The order user and AI moves depends on `symbolUser`.
        * `NOUGHT` goes first, `CROSS` goes second.

@parameters
    * board
        * Current game state to make turn in.
    * symbolUser
        * User symbol.
    * symbolAI
        * AI symbol.

@return
    * Indicates if current game has ended.
    * Game can end due to win, lose, draw or user quits.
*/
static bool turn(board_t *board,
                 char     symbolUser,
                 char     symbolAI)
{
    if (symbolUser == NOUGHT)
    {
        // user first, AI second
        if(!moveUser(board, symbolUser, symbolAI)
            || !moveAI(board, symbolUser, symbolAI))
        {
            return false;
        }
    }
    else
    {
        // user second, AI first
        if(!moveAI(board, symbolUser, symbolAI)
            || !moveUser(board, symbolUser, symbolAI))
        {
            return false;
        }
    }

    // turn did not result in end state (win/lose/draw) and user did not quit
    return true;
}


/*
@context
    * Makes a single move from the user.
    * User may quit instead of making a move.

@parameters
    * board
        * Current game state to make move in.
    * symbolUser
        * User symbol.
    * symbolAI
        * AI symbol.

@return
    * Indicates if current game has ended.
    * Game can end due to win, draw or user quits.
*/
static bool moveUser(board_t *board,
                     char     symbolUser,
                     char     symbolAI)
{
    char input;
    int cell;

    // get user input - may quit current game
    input = getInputLoop(board, symbolUser, false, true);
    if (input == KEY_QUIT)
    {
        updateMessage(MSG_REPLAY);
        return false;
    }

    // make user move - guaranteed to be valid
    cell = input - '0';
    setCell(board, cell, symbolUser);
    updateBoard(cell, symbolUser);

    return !isEndState(board, symbolUser, symbolAI);
}


/*
@context
    * Makes a single move from the AI.
    * Uses Minimax with Alpha-Beta pruning - optimal move.

@parameters
    * board
        * Current game state to make move in.
    * symbolUser
        * User symbol.
    * symbolAI
        * AI symbol.

@return
    * Indicates if current game has ended.
    * Game can end due to loss or draw.
*/
static bool moveAI(board_t *board,
                   char     symbolUser,
                   char     symbolAI)
{
    uint8_t cell;

    // get and make AI's move
    cell = getBestMove(board, symbolAI);
    setCell(board, cell, symbolAI);
    updateBoard(cell, symbolAI);

    return !isEndState(board, symbolUser, symbolAI);
}


/*
@context
    * Determines if a game has reached an end state.
    * A message is displayed to communicate the game state if end state reached.

@parameters
    * board
        * Current game state to check if end state.
    * symbolUser
        * User symbol.
    * symbolAI
        * AI symbol.

@return
    * Indicates if current game has ended.
*/
static bool isEndState(board_t *board,
                       char     symbolUser,
                       char     symbolAI)
{
    if (isWin(board, symbolUser))
    {
        updateMessage(MSG_WIN);
        return true;
    }
    else if (isWin(board, symbolAI))
    {
        updateMessage(MSG_LOSE);
        return true;
    }
    else if (isDraw(board))
    {
        updateMessage(MSG_DRAW);
        return true;
    }

    return false;
}


/*
@context
    * Gets the symbol of the user.
    * The symbol indicates what order user makes their move.
        * `NOUGHT` goes first, `CROSS` goes second.

@return
    * Symbol user will use for current game.
    * User can also quit instead of selecting a symbol.
*/
static char getSymbol()
{
    char input;

    updateMessage(MSG_ORDER);
    input = getInputLoop(NULL, EMPTY, true, false);
    if (input == KEY_QUIT)
    {
        return KEY_QUIT;
    }
    else if (input == KEY_YES)
    {
        return NOUGHT;
    }

    return CROSS;
}


/*
@context
    * Determines if user wants to play another game.

@return
    * Indicates if user wants to play another game.
*/
static bool isReplay()
{
    if (getInputLoop(NULL, EMPTY, true, false) == KEY_YES)
    {
        return true;
    }

    // no or quit
    return false;
}


/*
@context
    * Gets a valid input from user.
    * Repeatedly prompts the user until valid input entered.
    * Type of input changes depending on `isYesNo` and `isNumber`.
        * Quitting is always an option.

@parameters
    * board
        * Current game state to check if valid move when `isNumber` toggled.
    * symbol
        * User symbol.
    * isYesNo
        * Indicates if input in a yes or no question.
    * isNumber
        * Indicates if input move input - number range depends on `board` size.

@return
    * Valid input from user.
    * Quits if terminal too small for interface.
*/
static char getInputLoop(board_t *board,
                         char     symbol,
                         bool     isYesNo,
                         bool     isNumber)
{
    char input;
    int cell;

    do
    {
        // exit the interface if terminal becomes too small
        if (!isTerminalValidSize())
        {
            return KEY_QUIT;
        }

        input = getInput();
        cell = input - '0';
    }
    while (!(input == KEY_QUIT
        || (isYesNo && (input == KEY_YES || input == KEY_NO))
        || (isNumber && isdigit(input) && isValidMove(board, cell, symbol))));

    return input;
}
