CC = gcc
CFLAGS = -Wall -Werror -Wextra -I./includes
LDFLAGS = -linput -ludev

NAME = xgestured

SRCS = src/utils.c src/main.c src/ini.c

OBJS = $(SRCS:.c=.o)

HEADERS = includes/header.h includes/ini.h

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

install: $(NAME)
	@echo "Installing service..."
	install -Dm755 $(NAME) /usr/local/bin/$(NAME)
	install -Dm644 $(NAME).service /etc/systemd/system/$(NAME).service
	@echo "Success."
	systemctl enable --now $(NAME)

uninstall:
	@echo "Uninstalling..."
	systemctl disable --now $(NAME) || true
	rm -f /usr/local/bin/$(NAME)
	rm -f /etc/systemd/system/$(NAME).service
	@echo "Success."

.PHONY: all clean fclean re install uninstall