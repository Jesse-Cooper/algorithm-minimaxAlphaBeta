#include "interface.h"

#include <ctype.h>
#include <ncurses.h>
#include <stdio.h>


// text interface - bottom area is where messages are displayed
static const char INTERFACE[] =
    "###################################################################\n"
    "# Minimax Algorithm with Alpha-Beta Pruning (Noughts and Crosses) #\n"
    "#                                                                 #\n"
    "# Place your symbols on the left board using the corresponding    #\n"
    "# cell numbers on the right board                                 #\n"
    "#                                                                 #\n"
    "# Press Q in a game to end it early and outside of a game to quit #\n"
    "# Keep the terminal size larger than 68x19                        #\n"
    "###################################################################\n"
    "#               -------------         -------------               #\n"
    "#               |   |   |   |         | 0 | 1 | 2 |               #\n"
    "#               -------------         -------------               #\n"
    "#               |   |   |   |         | 3 | 4 | 5 |               #\n"
    "#               -------------         -------------               #\n"
    "#               |   |   |   |         | 6 | 7 | 8 |               #\n"
    "#               -------------         -------------               #\n"
    "###################################################################\n"
    "#                                                                 #\n"
    "###################################################################\n";

// size of the interface (terminal must be >= these dimensions)
static const int WIDTH_INTERFACE = 68;
static const int HEIGHT_INTERFACE = 19;

// position to place symbol in top-left board cell
static const int X_BOARD = 18;
static const int Y_BOARD = 10;

// position of bottom area to display messages
static const int X_MSG = 2;
static const int Y_MSG = 17;

// space between rows and columns of board
static const int SPACE_ROW = 2;
static const int SPACE_COLUMN = 4;

static const char MSG_ERROR_SIZE[] = "Terminal size must be at least %dx%d\n";

// used to clear the message space when displaying a new message
static const char MSG_BLANK[] =
    "                                                               ";


/* ------------------------------ START PUBLIC ------------------------------ */


/*
@context
    * Initialises and displays the ncurses text interface.
    * Will not initialise if terminal too small.

@return
    * Indicates if interface successfully initialised.
*/
bool initInterface()
{
    initscr();

    // do not initialise interface if terminal too small
    if (!isTerminalValidSize())
    {
        freeInterface();
        return false;
    }

    // hide cursor and characters typed
    curs_set(0);
    noecho();

    // display a blank interface
    printw(INTERFACE);
    refresh();

    return true;
}


/*
@context
    * Closes ncurses interface.
*/
void freeInterface()
{
    endwin();

    // exit was due to terminal being too small
    if (!isTerminalValidSize())
    {
        printf(MSG_ERROR_SIZE, WIDTH_INTERFACE, HEIGHT_INTERFACE);
    }
}


/*
@context
    * Gets a key input from the user.
    * Program is paused until the input is received.

@return
    * Key input from user
    * If input is a letter it becomes lowercase.
*/
char getInput()
{
    return tolower(getch());
}


/*
@context
    * Displays `symbol` on the interface board at `cell`.

@parameters
    * cell
        * Location to place `symbol`.
    * symbol
        * Symbol to place.
*/
void updateBoard(uint8_t cell,
                 char    symbol)
{
    uint8_t x, y;

    x = X_BOARD + (SPACE_COLUMN * (cell % BOARD_SIZE));
    y = Y_BOARD + (SPACE_ROW * (cell / BOARD_SIZE));
    mvaddch(y, x, symbol);
    refresh();
}


/*
@context
    * Displays `message` on the interface.

@parameters
    * message
        * Message to display.
*/
void updateMessage(const char message[])
{
    mvprintw(Y_MSG, X_MSG, MSG_BLANK);
    mvprintw(Y_MSG, X_MSG, message);
    refresh();
}


/*
@context
    * Checks if the terminal is a valid size for the interface.

@return
    * Indicates if the terminal is a valid size for the interface.
*/
bool isTerminalValidSize()
{
    return (COLS >= WIDTH_INTERFACE) && (LINES >= HEIGHT_INTERFACE);
}


/* ------------------------------- END PUBLIC ------------------------------- */
