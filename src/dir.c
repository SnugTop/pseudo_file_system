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

    // Read the root directory block (fixed at block 67)
    block_read(67, &dir_block);

    // Search through the directory entries
    for (int i = 0; i < dir_block.num_director_entries; ++i) {
        // If filename matches, return associated inode number

        if (strcmp(dir_block.director_entries[i].d_name, filename) == 0) {
            return dir_block.director_entries[i].d_ino;
        }
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

    // Read the root directory block (fixed at block 67)
    block_read(67, &dir_block);

    // Check if directory is already full
    if (dir_block.num_director_entries >= MAX_DIR_ENTRIES) {
        return -1; // No space left in directory
    }
    // Get pointer to next free directory entry
    DIR_ENTRY *new_entry = &dir_block.director_entries[dir_block.num_director_entries];

    // Fill in the directory entry fields
    strncpy(new_entry->d_name, filename, MAX_NAME_LENGTH); // Copy filename
    new_entry->d_ino = inode_number;    // Set inode number
    new_entry->d_type = type;            // Set type (1 = directory, 2 = file)
    new_entry->d_reclen = sizeof(DIR_ENTRY); // (Optional: record length)
    new_entry->d_off = 0;                // (Optional: not used for now)

    // Increase the number of directory entries
    dir_block.num_director_entries++;

    // Write updated directory block back to disk
    block_write(67, &dir_block);

    return 0; // Success
}

/*
 * pdos_dir:
 * Returns a list of file names (strings) currently in the root directory.
 * The returned list is NULL-terminated.
 * Caller is responsible for freeing the list and each string inside it.
 */
char **pdos_dir(void) {
    DIR_BLOCK dir_block;

    // Read the root directory block (fixed at block 67)
    block_read(67, &dir_block);

    //creates enough space for each element of directory
    int num_files = dir_block.num_director_entries;

    // Allocate an array of char* pointers, one per file + 1 for NULL at end
    char **list = malloc(sizeof(char *) * (num_files + 1));

    // Copy each filename into the list
    for (int i = 0; i < num_files; ++i) {
        list[i] = strdup(dir_block.director_entries[i].d_name); // strdup allocates new memory for each name
    }

    // Null-terminate the list
    list[num_files] = NULL;

    return list;
}
