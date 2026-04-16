CC = cc
CFLAGS = -Wall -Werror -Wextra -O2
LDFLAGS = -linput -ludev

NAME = xgestured

SRCS = utils.c main.c ini.c

OBJS = $(SRCS:.c=.o)

HEADERS = header.h ini.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

run: $(NAME)
	sudo ./$(NAME)

.PHONY: all clean fclean re install-deps run