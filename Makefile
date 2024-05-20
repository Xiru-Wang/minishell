RED=\033[1;31m
GREEN=\033[1;32m
RESET=\033[0m

# Executables
NAME = minishell
TEST_NAME = utest_minishell

# Compiler settings
CC = gcc
# CFLAGS = -Wall -Wextra -Werror -fsanitize=address -fsanitize=undefined -g
CFLAGS = -Wall -Wextra -Werror -g

# Directories
SRC_DIR = src
TEST_DIR = tests
INCLUDE_DIR = includes
LIBFT_DIR = libs/libft
CRITERION_INCLUDE_DIR = criterion/include
CRITERION_LIB_DIR = criterion/lib
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
TEST_SRCS = $(shell find $(TEST_DIR) -name 'test_*.c')

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.o,$(TEST_SRCS))

# Test dependencies (exclude main.o)
TEST_DEPS = $(filter-out $(OBJ_DIR)/main.o,$(OBJS))

# Library flags
LIBFT_FLAGS = -L$(LIBFT_DIR) -lft
CRITERION_FLAGS = -L$(CRITERION_LIB_DIR) -lcriterion

all: create_dirs $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_FLAGS) -I$(READLINE_PATH) $(READLINE_LIB) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(READLINE_PATH) -c $< -o $@

test: create_dirs $(TEST_NAME)

$(TEST_NAME): $(TEST_OBJS) $(TEST_DEPS)
	$(MAKE) -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(TEST_OBJS) $(TEST_DEPS) $(LIBFT_FLAGS) $(CRITERION_FLAGS) -I$(READLINE_PATH) $(READLINE_LIB) -o $(TEST_NAME)

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(CRITERION_INCLUDE_DIR) -I$(READLINE_PATH) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME) $(TEST_NAME)

re: fclean all

norm:
	@norminette ./src
	@norminette -R CheckForbiddenSourceHeader ./includes

val:
	@valgrind --track-fds=yes --leak-check=full ./minishell

.PHONY: all create_dirs test clean fclean re norm val
