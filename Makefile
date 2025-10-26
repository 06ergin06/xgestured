# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -Wextra -O2
LDFLAGS = -linput -ludev

# Project name
NAME = jestapp

# Source files
SRCS = utils.c main.c ini.c

# Object files
OBJS = $(SRCS:.c=.o)

# Header files
HEADERS = header.h ini.h

# Default target
all: $(NAME)

# Build the executable
$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files
clean:
	rm -f $(OBJS)

# Clean everything (object files and executable)
fclean: clean
	rm -f $(NAME)

# Rebuild everything
re: fclean all

# Install dependencies (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install wmctrl xdotool libinput-tools libinput-dev libudev-dev

# Run with sudo (required for input device access)
run: $(NAME)
	sudo ./$(NAME)

# Phony targets
.PHONY: all clean fclean re install-deps run
