# Minimax Algorithm with Alpha-Beta Pruning

This project demonstrates an application of the Minimax algorithm with Alpha-Beta pruning in a standard `3x3` Noughts and Crosses game.

## Application

A C program that allows the user to play standard `3x3` Noughts and Crosses games against an optimal Artificial intelligence (AI).

As the AI is optimal the user can never win a game only ever draw or lose.

### Invoking Instructions

The program can be compiled using the `Makefile` (compiled with `make`) and run with `./program`.

At the start of a game you must select your symbol of either Noughts (`O`) or Crosses (`X`) where Noughts move first and Crosses second.

There is a numbered board on the right of the interface indicating numbers to enter to place you symbol in the corresponding cells.

Press `Q` in a game to end it early and outside of a game to quit.
The terminal size must be at least `68x19` for the program to run.

### Optimal Move (Minimax Algorithm with Alpha-Beta Pruning)

Each turn the AI will enumerate all possible moves and determine the move which leads to a win (or draw if win not possible).

The depth of the Minimax search tree is used to encourage the AI to make wins using the least amount of moves.
