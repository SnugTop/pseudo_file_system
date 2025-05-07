# Compiler and flags
CC        = gcc
CFLAGS    = -std=c11 -Wall -Wextra -pedantic -D_XOPEN_SOURCE=700 -I$(INCLUDE_DIR)

# Detect platform for librt
UNAME_S   := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    LDFLAGS = -lrt
else
    LDFLAGS =
endif

# Directories
SRC_DIR     = src
INCLUDE_DIR = include
BUILD_DIR   = build
LIB_DIR     = lib
TEST_DIR    = test

# Static library name 
STATIC_LIB  = $(LIB_DIR)/libPseudoFS.a

# Library source files 
LIB_SRCS    = \
    $(SRC_DIR)/disk.c    \
    $(SRC_DIR)/inode.c   \
    $(SRC_DIR)/dir.c     \
    $(SRC_DIR)/fs.c

LIB_OBJS    = $(LIB_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Wrapper commands
WRAPPERS      = pdos_mkdisk pdos_mkfs pdos_dir
WRAPPER_SRCS  = $(addprefix $(SRC_DIR)/, $(addsuffix .c,$(WRAPPERS)))

# Tests
TESTS       = \
    test_mkdisk        \
    test_inode_alloc   \
    test_dir_ops       \
    test_persistence   \
    test_write_file1   \
    test_write_file2   \
    test_read_file1    \
    test_grow_file1    \
    test_grow_file2    \
    test_print_files   \
    test_subdir        \
    for_submission

TEST_PROGS  = $(addprefix $(TEST_DIR)/,$(TESTS))

# Default target builds library, wrappers, and tests
all: $(STATIC_LIB) $(WRAPPERS) $(TEST_PROGS)

# Build object files from library sources
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Archive static library 
$(STATIC_LIB): $(LIB_OBJS) | $(LIB_DIR)
	ar rcs $@ $^
	ranlib $@

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

# Links against libPseudoFS.a and -lrt
$(WRAPPERS): %: $(SRC_DIR)/%.c $(STATIC_LIB)
	$(CC) $(CFLAGS) $< -L$(LIB_DIR) -lPseudoFS -o $@ $(LDFLAGS)

# Build test programs (still link against the same library)
$(TEST_DIR)/%: $(TEST_DIR)/%.c $(STATIC_LIB)
	$(CC) $(CFLAGS) $< -L$(LIB_DIR) -lPseudoFS -o $@ $(LDFLAGS)

# Shortcut to rebuild just tests
tests: $(TEST_PROGS)

# Clean up everything
clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)/libPseudoFS.a $(WRAPPERS) $(TEST_PROGS)

.PHONY: all tests clean
