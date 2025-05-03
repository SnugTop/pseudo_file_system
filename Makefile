# Compiler and flags
CC = gcc
CFLAGS = -std=c11 -Wall -D_POSIX_C_SOURCE=200809L

# Directories
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = test

# Detect if we're on Linux
OS := $(shell uname)
ifeq ($(OS), Linux)
    LDFLAGS = -lrt
else
    LDFLAGS =
endif

# Test programs
TESTS = test_mkdisk test_inode test_dir

# Default target
all: $(addprefix $(TEST_DIR)/, $(TESTS))

# How to build each test
$(TEST_DIR)/test_mkdisk: $(TEST_DIR)/test_mkdisk.c $(SRC_DIR)/disk.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $^ -o $@ $(LDFLAGS)

$(TEST_DIR)/test_inode: $(TEST_DIR)/test_inode.c $(SRC_DIR)/disk.c $(SRC_DIR)/inode.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $^ -o $@ $(LDFLAGS)

$(TEST_DIR)/test_dir: $(TEST_DIR)/test_dir.c $(SRC_DIR)/disk.c $(SRC_DIR)/inode.c $(SRC_DIR)/dir.c $(SRC_DIR)/fs.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $^ -o $@ $(LDFLAGS)


# Clean everything
clean:
	rm -f $(TEST_DIR)/test_mkdisk
	rm -f $(TEST_DIR)/test_inode
	rm -f $(TEST_DIR)/test_dir

.PHONY: all clean
