# Compiler and flags
CC = gcc
CFLAGS = -std=c11 -Wall

# Directories
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = test

# Detect if we're on Linux (uname = Linux)
OS := $(shell uname)
ifeq ($(OS), Linux)
    LDFLAGS = -lrt
else
    LDFLAGS =
endif

# Default target
all: $(TEST_DIR)/test_mkdisk $(TEST_DIR)/test_inode

# Rule for test_mkdisk
$(TEST_DIR)/test_mkdisk: $(TEST_DIR)/test_mkdisk.c $(SRC_DIR)/disk.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $^ -o $@ $(LDFLAGS)

# Rule for test_inode
$(TEST_DIR)/test_inode: $(TEST_DIR)/test_inode.c $(SRC_DIR)/disk.c $(SRC_DIR)/inode.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $^ -o $@ $(LDFLAGS)

# Clean build artifacts
clean:
	rm -f $(TEST_DIR)/test_mkdisk
	rm -f $(TEST_DIR)/test_inode

.PHONY: all clean
