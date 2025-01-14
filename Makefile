# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g
LDFLAGS = -lgmp

# Source files
SRC = src/main.c src/prime_utils.c src/math_utils.c

# Output binary
EXEC = tst

# Build the program
all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Clean up generated files
clean:
	rm -f $(EXEC)

# Phony targets to avoid conflict with files
.PHONY: all clean
