HOSTTYPE ?= $(shell uname -m)_$(shell uname -s)

NAME = libft_malloc_$(HOSTTYPE).so
LINK = libft_malloc.so

TEST = test_stress_malloc
TEST_SRC = test_stress_malloc.c

CC = cc

CFLAGS = -Wall -Wextra -Werror -fPIC
LDFLAGS = -shared

INCLUDES = -Iincludes -Ilibft/includes

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCDIR = src

SRC = \
	$(SRCDIR)/malloc.c \
	$(SRCDIR)/malloc_utils.c \
	$(SRCDIR)/free.c \
	$(SRCDIR)/realloc.c \
	$(SRCDIR)/show_alloc_mem.c

OBJ = $(SRC:.c=.o)


all: $(NAME)


$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)


$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(LDFLAGS) $(OBJ) $(LIBFT) -o $(NAME)
	ln -sf $(NAME) $(LINK)


test: $(NAME) $(TEST)


$(TEST): $(TEST_SRC)
	$(CC) -Wall -Wextra -Werror $(INCLUDES) $(TEST_SRC) \
		-L. -lft_malloc \
		-Wl,-rpath,'$$ORIGIN' \
		-o $(TEST)


%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@


clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean


fclean: clean
	rm -f $(NAME) $(LINK) $(TEST)
	$(MAKE) -C $(LIBFT_DIR) fclean


re: fclean all


.PHONY: all test clean fclean re