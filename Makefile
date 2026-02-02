CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g

# Targets
all: str_replace test_str_replace

# Main program
str_replace: main.c str_replace.c str_replace.h
	$(CC) $(CFLAGS) -o str_replace main.c str_replace.c

# Test program
test_str_replace: test_str_replace.c str_replace.c str_replace.h
	$(CC) $(CFLAGS) -o test_str_replace test_str_replace.c str_replace.c

# Run tests
test: test_str_replace
	./test_str_replace

# Clean build artifacts
clean:
	rm -f str_replace test_str_replace

.PHONY: all test clean
