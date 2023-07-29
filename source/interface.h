/*
@context
    * Provides a text interface to play Noughts and Crosses.
        * Also allows key inputs to be received from the user.
    * The terminal must be >= than the interface dimensions.
*/


#ifndef _INTERFACE_H
    #define _INTERFACE_H

    #include <stdbool.h>
    #include <stdint.h>


    // standard 3x3 Noughts and Crosses size
    static const int BOARD_SIZE = 3;

    // messages displayed on interface to communicate a game's status
    static const char MSG_ORDER[] = "Do you want to go first (Y or N)?";
    static const char MSG_MOVE[] = "What is your move (0 to 8)?";
    static const char MSG_WIN[] = "You WON! Play again (Y or N)?";
    static const char MSG_LOSE[] = "You LOSE! Play again (Y or N)?";
    static const char MSG_DRAW[] = "You DREW! Play again (Y or N)?";
    static const char MSG_REPLAY[] = "Play another game (Y or N)?";


    bool initInterface();
    void freeInterface();

    char getInput();

    void updateBoard(uint8_t cell,
                     char    symbol);

    void updateMessage(const char message[]);

    bool isTerminalValidSize();

#endif
