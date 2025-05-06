# Compiler and flags
CC      = gcc
CFLAGS  = -std=c11 -Wall -D_POSIX_C_SOURCE=200809L

# Detect platform
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    LDFLAGS = -lrt
else
    LDFLAGS =
endif

# Directories
SRC_DIR     = src
INCLUDE_DIR = include
TEST_DIR    = test
BUILD_DIR   = build
LIB_DIR     = lib
STATIC_LIB  = $(LIB_DIR)/libpdosfs.a

# Source files
SRCS = $(SRC_DIR)/disk.c $(SRC_DIR)/inode.c $(SRC_DIR)/dir.c $(SRC_DIR)/fs.c
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Test programs
TESTS = test_mkdisk test_inode_alloc test_dir_ops test_persistence \
        test_write_file1 test_write_file2 test_read_file1 \
        test_grow_file1 test_grow_file2 test_print_files test_subdir \
        for_submission

# Default target
all: $(STATIC_LIB) $(addprefix $(TEST_DIR)/, $(TESTS))

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Archive static library
$(STATIC_LIB): $(OBJS)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

# Rule for building test programs using the static lib
$(TEST_DIR)/%: $(TEST_DIR)/%.c $(STATIC_LIB)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $< -L$(LIB_DIR) -lpdosfs -o $@ $(LDFLAGS)

# Recompile only tests
tests: $(addprefix $(TEST_DIR)/, $(TESTS))

# Clean targets
clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)/libpdosfs.a $(addprefix $(TEST_DIR)/, $(TESTS))

clean_tests:
	rm -f $(addprefix $(TEST_DIR)/, $(TESTS))

.PHONY: all clean clean_tests tests
