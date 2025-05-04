//dir.c
#include "dir.h"
#include "defs.h"
#include "disk.h"
#include <string.h>
#include <stdlib.h>

/*
 * dir_lookup:
 * Searches the root directory for a file by name.
 * Returns the inode number if found, -1 if not found.
 */
int dir_lookup(const char *filename) {
    DIR_BLOCK dir_block;
    unsigned short cur_block = 67;

    // looks at each directory entry in turn in each block
    while (cur_block < 1091) {
        block_read(cur_block, &dir_block); // initially 67 = first data block for root dir
        for (int i = 0; i < dir_block.num_dir_entries; ++i) {
            // If filename matches, return associated inode number
            if (strcmp(dir_block.dir_entries[i].d_name, filename) == 0) {
                return dir_block.dir_entries[i].d_ino; // Return inode number
            }
        }
        cur_block++;
    }

    // File not found
    return -1;
}

/*
 * dir_add:
 * Adds a new file (or directory) entry into the root directory.
 * Parameters:
 * - filename: name of the file to add
 * - inode_number: inode number to associate with this file
 * - type: 1 for directory, 2 for regular file
 * Returns 0 on success, -1 if directory is full.
 */
int dir_add(const char *filename, int inode_number, int type) {
    DIR_BLOCK dir_block;
    unsigned short cur_block = 67;

    // Search for a block with space
    while (cur_block < 1091) {
        block_read(cur_block, &dir_block);

        if (dir_block.num_dir_entries < MAX_DIR_ENTRIES) {
            DIR_ENTRY *new_entry = &dir_block.dir_entries[dir_block.num_dir_entries];

            strncpy(new_entry->d_name, filename, MAX_NAME_LENGTH);
            new_entry->d_ino = inode_number;
            new_entry->d_type = type;
            new_entry->d_reclen = sizeof(DIR_ENTRY);
            new_entry->d_off = 0;

            dir_block.num_dir_entries++;
            block_write(cur_block, &dir_block);
            return 0;
        }

        cur_block++;
    }

    return -1; // Directory full
}

/*
 * pdos_dir:
 * Returns a list of file names (strings) currently in the root directory.
 * The returned list is NULL-terminated.
 * Caller is responsible for freeing the list and each string inside it.
 */
char **pdos_dir(void) {
    DIR_BLOCK dir_block;
    int total_files = 0;

    // First pass to count total files
    for (int block = 67; block < 1091; ++block) {
        block_read(block, &dir_block);
        total_files += dir_block.num_dir_entries;
    }

    char **list = malloc(sizeof(char *) * (total_files + 1)); // +1 for NULL terminator
    if (!list) return NULL;

    int index = 0;
    for (int block = 67; block < 1091; ++block) {
        block_read(block, &dir_block);
        for (int i = 0; i < dir_block.num_dir_entries; ++i) {
            list[index++] = strdup(dir_block.dir_entries[i].d_name);
        }
    }

    list[total_files] = NULL;
    return list;
}
