CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = pipex
DEBUG = d_pipex
LIB = Libft/libft.a
SRC = pipex.c \
      pipe_helpers.c \
      children_helpers.c \
      program_helpers.c
OBJ = ${SRC:.c=.o}

all: $(NAME)

debug: $(DEBUG)

$(NAME): $(LIB) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME)

$(DEBUG): $(LIB) $(SRC)
	$(CC) -g $(SRC) $(LIB) -o $(DEBUG)

$(LIB):
	$(MAKE) -C Libft
%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm $(NAME)

oclean:
	rm $(LIB) $(OBJ)
	$(MAKE) -C Libft fclean

fclean: clean oclean

re: fclean all

.PHONY: all clean fclean re debug
