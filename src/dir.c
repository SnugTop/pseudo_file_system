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

    //looks at each directory entry in turn in each block
    while (cur_block < 1091) {
        block_read(cur_block, &dir_block); // initially 67 = first data block for root dir
        for (int i = 0; i < dir_block.num_director_entries; ++i) {
            // If filename matches, return associated inode number
            //compares name attribute for each entry to filename
            if (strcmp(dir_block.director_entries[i].d_name, filename) == 0) {
                return dir_block.director_entries[i].d_ino; // Return inode number
            }
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
    unsigned short cur_block = 67;
    // Read the root directory block (fixed at block 67)
    block_read(cur_block, &dir_block);

    // Check if directory is already full
    if (dir_block.num_director_entries >= MAX_DIR_ENTRIES) {
        cur_block++;
        block_read(cur_block, &dir_block);
        if (cur_block == 1091) {
            return -1; // Directory full
        }
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


    block_write(cur_block, &dir_block);
    return 0;

}

/*
 * pdos_dir:
 * Returns a list of file names (strings) currently in the root directory.
 * The returned list is NULL-terminated.
 * Caller is responsible for freeing the list and each string inside it.
 */
char **pdos_dir(void) {
    DIR_BLOCK dir_block;

    unsigned short u_s = data_blocks_used; //for different blocks

    int tot_size = 0; //total number of elements across blocks
    for (short count = 0; count < u_s; count++) {
        block_read(67+count, &dir_block);
        
        //creates enough space for each element of directory
        int num_files = dir_block.num_director_entries;
        tot_size += num_files;
    }

    //creates enough space for each element of directory
    char **list = malloc(sizeof(char *) * (tot_size + 1)); // +1 for NULL at end

    //lists the name of each in turn
    int cur_index = 0;
    for (int i = 0; i < data_blocks_used; i++) {
        block_read(67, &dir_block);
        for (int j = 0; j < dir_block.num_director_entries; j++) {
            list[cur_index] = strdup(dir_block.director_entries[i].d_name);
        }
        cur_index++;

    }

    // Null-terminate the list
    list[num_files] = NULL;

    return list;
}
