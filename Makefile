CC = gcc
CFLAGS = -Wall -Werror -Wextra -I./includes
LDFLAGS = -linput -ludev

NAME = xgestured

SRCS = src/utils.c src/main.c src/ini.c

OBJS = $(SRCS:.c=.o)

HEADERS = includes/header.h includes/ini.h

BIN_DIR = $(HOME)/.local/bin
SERV_DIR = $(HOME)/.config/systemd/user
CONF_DIR = /etc/$(NAME)

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
	@if [ "$$(id -u)" -eq 0 ]; then \
		echo "Error: You should 'make install' without 'sudo'; \
		exit 1; \
	fi
	@echo "Installing service..."
	mkdir -p $(BIN_DIR)
	mkdir -p $(SERV_DIR)
	install -m 755 $(NAME) $(BIN_DIR)/$(NAME)
	install -m 644 $(NAME).service $(SERV_DIR)/$(NAME).service
	sudo mkdir -p $(CONF_DIR)
	sudo install -m 644 config.ini $(CONF_DIR)/config.ini
	sudo usermod -aG input $$USER
	systemctl --user daemon-reload
	systemctl --user enable --now $(NAME)
	@echo ""
	@echo "=================================================================="
	@echo "SUCCESS!"
	@echo "⚠️ WARNING"
	@echo "PLEASE REBOOT OR LOG OUT SESSION."
	@echo "=================================================================="

uninstall:
	@echo "Uninstalling service..."
	systemctl --user disable --now $(NAME) || true
	rm -f $(BIN_DIR)/$(NAME)
	rm -f $(SERV_DIR)/$(NAME).service
	sudo rm -rf $(CONF_DIR)
	@echo "Success"

.PHONY: all clean fclean re install uninstall