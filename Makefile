# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Target executable
TARGET = exe

# Source files
SRCS = gen.c main.c menu.c player.c

# Object files (replace .c with .o)
OBJS = $(SRCS:.c=.o)

# Default rule
all: $(TARGET)

# Rule to link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm -f $(TARGET) $(OBJS)

# Phony targets to avoid conflicts with files named 'all' or 'clean'
.PHONY: all clean
