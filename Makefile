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
	@if [ "$$(id -u)" -ne 0 ]; then \
		echo "Error: make install must be run as root (sudo make install)"; \
		exit 1; \
	fi
	@echo "Installing service..."
	install -Dm755 $(NAME) /usr/local/bin/$(NAME)
	install -Dm644 $(NAME).service /etc/systemd/system/$(NAME).service
	install -Dm644 config.ini /etc/$(NAME)/config.ini
	systemctl enable --now $(NAME)
	@echo "Success."

uninstall:
	@echo "Uninstalling..."
	systemctl disable --now $(NAME) || true
	rm -f /usr/local/bin/$(NAME)
	rm -f /etc/systemd/system/$(NAME).service
	rm -rf /etc/$(NAME)
	@echo "Success."

.PHONY: all clean fclean re install uninstall