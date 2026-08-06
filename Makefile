NAME = libft_malloc_$(HOSTTYPE).so
LINK = libft_malloc.so

CC = cc
CFLAGS = -Wall -Wextra -Werror -fPIC
LDFLAGS = -shared

ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname - m)_$(shell uname -s)
endif

INCLUDES = -Iincludes

SRCDIR = src

SRC = \
	$(SRCDIR)/malloc.c \
	$(SRCDIR)/free.c \
	$(SRCDIR)/realloc.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $(NAME)
	ln -sf $(NAME) $(LINK)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean:
	rm -f $(NAME) $(LINK)

re: fclean all

.PHONY: all clean fclean re