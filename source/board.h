/*
@context
    * Provides a data structure to play a game of Noughts and Crosses.
    * Although Noughts and Crosses is usually a 3x3 game it can be set to any
      size with this data structure.
    * To win user must fill a row, column or diagonal with their symbol.
*/


#ifndef _BOARD_H
    #define _BOARD_H

    #include <stdbool.h>
    #include <stdint.h>


    static const char NOUGHT = 'O';
    static const char CROSS = 'X';

    // used to unmake moves
    static const char EMPTY = ' ';


    typedef struct board_s board_t;


    board_t *initBoard(uint8_t size);
    void freeBoard(board_t *board);

    void resetBoard(board_t *board);

    bool isWin(board_t *board,
               char     symbol);
    bool isDraw(board_t *board);

    bool isValidMove(board_t *board,
                     uint8_t  cell,
                     char     symbol);

    uint8_t getSize(board_t *board);
    char getCell(board_t *board,
                 uint8_t  cell);

    void setCell(board_t *board,
                 uint8_t  cell,
                 char     symbol);

#endif
