NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -fPIC

INCLUDES = -Iincludes

SRCDIR = src

SRC = \
	$(SRCDIR)/ft_put_address_hexa.c \
	$(SRCDIR)/ft_putchar.c \
	$(SRCDIR)/ft_putnbr.c \
	$(SRCDIR)/ft_putstr.c \
	$(SRCDIR)/ft_strlen.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re