// src/pdos_dir.c
#include "disk.h"
#include "dir.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    // 1) map the already‐created disk into memory
    if (pdos_mkdisk(false) != 0) {
        fputs("ERROR: failed to map existing disk. Did you run pdos_mkdisk?\n", stderr);
        return 1;
    }

    if (argc == 2) {
        char* directory_name = argv[1]; //specified parameter
        short inode_loc = dir_lookup(directory_name); //where it is
        if (inode_loc == -1) {
            fputs("ERROR: directory name %s does not exist\n", directory_name);
            return 1;
        }
        INODE_ENTRY i_entry;
        inode_read(inode_loc, &i_entry);
        short db_used = i_entry.data_blocks_used; //data blocks to iterate through
        for (int count = 0; count < db_used; count++) {
            DIR_BLOCK d_block;
            block_read(i_entry.data_blocks[count], &d_block); //get block info
            for (int counter = 0; counter < d_block.num_dir_entries; counter++) {
                char* contents = d_block.dir_entries[count].d_name; //name of item in directory
                printf("%s/n", contents);
            }
        }
        return 0;
    }

    // 2) read the directory entries
    char **listing = pdos_dir();
    if (!listing) {
        fputs("Failed to read directory.\n", stderr);
        return 1;
    }

    // 3) print them
    for (int i = 0; listing[i]; ++i) {
        printf("%s\n", listing[i]);
        free(listing[i]);
    }
    free(listing);
    return 0;
}
