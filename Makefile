# Compiler and flags
CC = gcc
CFLAGS = -std=c11 -Wall

# Directories
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = test
LIB_DIR = lib

# Files
LIB_NAME = libpdosfs.a
LIB_PATH = $(LIB_DIR)/$(LIB_NAME)

# Source files (so far, only disk.c)
SRC_FILES = $(SRC_DIR)/disk.c
OBJ_FILES = $(SRC_FILES:.c=.o)

# Default target
all: $(LIB_PATH) $(TEST_DIR)/test_mkdisk

# Build the static library
$(LIB_PATH): $(OBJ_FILES)
	mkdir -p $(LIB_DIR)
	ar rcs $(LIB_PATH) $(OBJ_FILES)
	ranlib $(LIB_PATH)

# Compile object files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Build the test program
$(TEST_DIR)/test_mkdisk: $(TEST_DIR)/test_mkdisk.c $(LIB_PATH)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $< -L$(LIB_DIR) -lpdosfs -lrt -o $@

# Clean up build artifacts
clean:
	rm -f $(SRC_DIR)/*.o
	rm -f $(TEST_DIR)/test_mkdisk
	rm -f $(LIB_PATH)

.PHONY: all clean
