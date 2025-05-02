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
    int div = loc_file >>> 4; //(loc_file)/16
    int block = 3 + div; //which block it's in
    int loc = loc_file & 15; //which inode within the block
    PDOS_FILE pf;
    pf.inode_block = block; //starting at 3 to account for prior blocks
    pf.inode_index = loc;
    pf.data_block_cur= 0;
    pf.loc_data_in_block = 0;
    return &pf;
}

int pdos_fgetc(PDOS_FILE * pf) {
    unsigned short ib = pf->inode_block;
    unsigned short ie = pf->inode_index; 
    INODE_BLOCK i_block;
    block_read(ib, &i_block);
    INODE_ENTRY* i_entry = &i_block.inodes[ie];
    if (i_entry->size <= pf->data_block_cur*1024 + pf->loc_data_in_block) {
        //if loc_data_in_block is 500, then if there are 500 in block or fewer, top index will be 499 -- i.entry.size > 500
        //thus <= (i_entry.size and data_block_cur*1024 account for what's in prior blocks)
        return EOF;
    }
    DATA_BLOCK d_block;
    block_read(67 + pf->data_block_cur, &d_block);
    //incrementation
    pf->loc_data_in_block = pf->loc_data_in_block + 1;
    if (pf->loc_data_in_block == 1024) {
        //going to next block
        pf->loc_data_in_block = 0;
        pf->data_block_cur = pf->data_block_cur + 1;
    }
    return d_block.data[pf->loc_data_in_block];
}

void pdos_fputc(int b, PDOS_FILE * pf) {
    char c = (short) b; //idk why it entered as an int
    unsigned short ib = pf->inode_block;
    unsigned short ie = pf->inode_index; 
    INODE_BLOCK i_block;
    block_read(ib, &i_block);
    INODE_ENTRY i_entry = i_block.inodes[ie];
    DATA_BLOCK d_block;
    block_read(67 + pf->data_block_cur, &d_block);
    if (i_entry.size < pf->data_block_cur*1024 + pf->loc_data_in_block) {
        //beyond end of file
        //do nothing
    }
    else if (i_entry.size == pf->data_block_cur*1024 + pf->loc_data_in_block) {
        i_block.inodes[ie].size = i_block.inodes[ie].size + 1; //increase size
    } //else do not increase size
    d_block.data[pf->loc_data_in_block] = c;
    //incrementation
    pf->loc_data_in_block = pf->loc_data_in_block + 1;
    if (pf->loc_data_in_block == 1024) {
    //going to next block
    pf->loc_data_in_block = 0;
    pf->data_block_cur = pf->data_block_cur + 1;
}

void pdos_fclose(PDOS_FILE *) {
}

void pdos_mkdir(char *dir) {
}
