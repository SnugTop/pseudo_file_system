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
TESTS = test_mkdisk test_inode_alloc test_dir_ops test_persistence

# Default target
all: $(addprefix $(TEST_DIR)/, $(TESTS))

# Individual test build rules
$(TEST_DIR)/test_mkdisk: $(TEST_DIR)/test_mkdisk.c $(SRC_DIR)/disk.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $^ -o $@ $(LDFLAGS)

$(TEST_DIR)/test_inode_alloc: $(TEST_DIR)/test_inode_alloc.c $(SRC_DIR)/disk.c $(SRC_DIR)/inode.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $^ -o $@ $(LDFLAGS)

$(TEST_DIR)/test_dir_ops: $(TEST_DIR)/test_dir_ops.c $(SRC_DIR)/disk.c $(SRC_DIR)/inode.c $(SRC_DIR)/dir.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $^ -o $@ $(LDFLAGS)

$(TEST_DIR)/test_persistence: $(TEST_DIR)/test_persistence.c $(SRC_DIR)/disk.c $(SRC_DIR)/inode.c $(SRC_DIR)/dir.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $^ -o $@ $(LDFLAGS)

# Clean build artifacts
clean:
	rm -f $(addprefix $(TEST_DIR)/, $(TESTS))

.PHONY: all clean
