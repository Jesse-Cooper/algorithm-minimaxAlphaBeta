# whole program compiled with `make` command and run with `./program`

CC = gcc -std=c17 -O3 -Wall -Wextra -o

NAME = program

SRC = main.c \
      board.c \
      interface.c \
      minimax.c

OBJ = $(SRC:.c=.o)

INCLUDES = -lncurses


# creates the program combining all files of `SRC`
$(NAME): $(OBJ)
	$(CC) $(NAME) $(OBJ) $(INCLUDES)


# compiles each `SRC` file into an object file
%.o: %.c
	$(CC) $@ $^ -c
