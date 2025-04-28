#include "dir.h"
#include "defs.h"
#include "disk.h"
#include <string.h>
#include <stdlib.h>

int dir_lookup(const char *filename) {
    DIR_BLOCK dir_block;
    block_read(67, &dir_block); // 67 = first data block for root dir

    for (int i = 0; i < dir_block.num_director_entries; ++i) {
        if (strcmp(dir_block.director_entries[i].d_name, filename) == 0) {
            return dir_block.director_entries[i].d_ino; // Return inode number
        }
    }

    return -1; // Not found
}

int dir_add(const char *filename, int inode_number, int type) {
    DIR_BLOCK dir_block;
    block_read(67, &dir_block);

    if (dir_block.num_director_entries >= MAX_DIR_ENTRIES) {
        return -1; // Directory full
    }

    DIR_ENTRY *new_entry = &dir_block.director_entries[dir_block.num_director_entries];
    strncpy(new_entry->d_name, filename, MAX_NAME_LENGTH);
    new_entry->d_ino = inode_number;
    new_entry->d_type = type;
    new_entry->d_reclen = sizeof(DIR_ENTRY);
    new_entry->d_off = 0; // optional

    dir_block.num_director_entries++;

    block_write(67, &dir_block);
    return 0;
}

char **pdos_dir(void) {
    DIR_BLOCK dir_block;
    block_read(67, &dir_block);

    int num_files = dir_block.num_director_entries;
    char **list = malloc(sizeof(char *) * (num_files + 1)); // +1 for NULL at end

    for (int i = 0; i < num_files; ++i) {
        list[i] = strdup(dir_block.director_entries[i].d_name);
    }

    list[num_files] = NULL; // Null-terminate list
    return list;
}
