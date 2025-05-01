#include "dir.h"
#include "defs.h"
#include "disk.h"
#include "inode.h"
#include "fs.h"

PDOS_FILE *pdos_open(const char *fname, const char
*mode) {
    //looks for where file is stored
    int loc_file = dir_lookup(fname);
    //if new file to be created
    if (loc_file == -1) {
        //looks for first free inode
        loc_file = inode_allocate(void);
    }
    //if file already exists then loc_file == inode number
    dir_add(filename, loc_file, FILE_TYPE); //new file added in
    int div = loc_file & (loc_file & 16); //(loc_file)/16
    int block = 3 + div; //which block it's in
    int loc = loc_file & 16; //which inode within the block
    INODE_BLOCK i_block;
    block_read(block, i_block);
    INODE_ENTRY inode;
    inode = i_block.inodes[loc];
    PDOS_FILE pf;
    PDOS_FILE * ret = PDOS_FILE* (first_data_block);
    return ret;
}

int pdos_fgetc(PDOS_FILE *) {
    //
}

int pdos_fputc(int b, PDOS_FILE *) {
    //
}
