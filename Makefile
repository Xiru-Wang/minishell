RED=\033[1;31m
GREEN=\033[1;32m
RESET=\033[0m

# Executables
NAME = minishell

# Compiler settings
CC = gcc
# CFLAGS = -Wall -Wextra -Werror -fsanitize=address -fsanitize=undefined -g
CFLAGS = -Wall -Wextra -Werror -g

# Directories
SRC_DIR = src
INCLUDE_DIR = includes
LIBFT_DIR = libs/libft
OBJ_DIR = obj

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin) # macOS
    READLINE_PATH = $(shell brew --prefix readline)/include
    READLINE_LIB_PATH = $(shell brew --prefix readline)/lib
    READLINE_LIB = -L$(READLINE_LIB_PATH) -lreadline
else # Assuming Linux or other Unix-like OS
    READLINE_PATH = /usr/include/readline
    READLINE_LIB = -lreadline
endif

# Source files
SRCS = $(shell find $(SRC_DIR) -name '*.c')

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Library flags
LIBFT_FLAGS = -L$(LIBFT_DIR) -lft

all: create_dirs $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_FLAGS) -I$(READLINE_PATH) $(READLINE_LIB) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(READLINE_PATH) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

norm:
	@norminette -R CheckForbiddenSourceHeader ./src
	@norminette -R CheckForbiddenSourceHeader ./libs/libft
	@norminette -R CheckForbiddenSourceHeader ./includes

val:
	@valgrind --track-fds=yes --leak-check=full ./minishell

.PHONY: all create_dirs clean fclean re norm val
