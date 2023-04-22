# Define the C compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Define the target executable name
TARGET = Shell

# Define source and object files
SRC = Shell.c
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Link object files to create the target executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the build directory
clean:
	rm -f $(OBJ) $(TARGET)

