RED=\033[1;31m
GREEN=\033[1;32m
RESET=\033[0m

NAME = minishell
CC = cc
CFLAGS = -Wall -Werror -Wextra

PATH_LIBFT = libft
LIBFT = $(PATH_LIBFT)/libft.a
SRC = $(wildcard *.c)
OBJS = $(SRC:.c=.o)

# Add here any additional libraries needed
#READLINE_LIB = -lreadline -lhistory -ltermcap
# Include directories for header files
INCLUDES = -Iincludes -I$(PATH_LIBFT)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(INCLUDES)
#$(READLINE_LIB)
# Make sure libft is compiled before compiling minishell
$(LIBFT):
	echo "Making libft..."
	$(MAKE) -C $(PATH_LIBFT)

clean:
	echo "Removing .o object files..."
	rm -f $(OBJS)
	$(MAKE) fclean -C $(PATH_LIBFT)

fclean: clean
	echo "Removing minishell..."
	rm -f $(NAME)

re: fclean all

# You might want to add a rule for .c.o to specify how .c files are compiled into .o files.
# For example:
# %.o: %.c
#   $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: all clean fclean re
