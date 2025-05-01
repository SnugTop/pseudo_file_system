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
    pf.inode_block = block;
    pf.inode_index = loc;
    pf.data_block_used = 0;
    pf.loc_data_in_block = 0;
    return &pf;
}

int pdos_fgetc(PDOS_FILE * pf) {
    short ib = pf->inode_block;
    short ie = pf->inode_entry; 
    INODE_BLOCK i_block;
    block_read(3 + ib, &i_block);
    INODE_ENTRY i_entry = i_block.inodes[ie];
    short num_d_blocks = pf->data_block_cur;
    short loc_data = pf->loc_data_in_block; //can be at maximum 32*20 = 640
    if (i_block == i_entry.data_blocks_used){
        //e.g. i_block is 6 but only 6 blocks are used
        return EOF;
    }
    DIR_BLOCK d_block;
    block_read(i_entry.data_blocks[i_block], &d_block);
    short d_e = loc_data >>> 5; //DIR_ENTRY within DIR_BLOCK
    short d_r = loc_data & 12; //location within dir_entry
    if (d_e == d_block.num_directory_entries) {
        //e.g. d_e is 6 but only 6 entries are used
        return EOF;
    }
    DIR_ENTRY d_entry = d_block.director_entries[d_e];
    if (d_r == d_entry.d_reclen) {
        return EOF;
    }
    //update PDOS_FILE*
    //32*20 = 640 bytes of data within each d_block
    pf->loc_data_in_block++;
    if (pf->loc_data_in_block == 640) {
        pf->loc_data_in_block = 0;
        //requires the allocation of a new data block
        pf->data_block_cur++; //moving onto new data block
    }
    return d_entry.data[d_r];
}

void pdos_fputc(int b, PDOS_FILE * pf) {
    pf->buffer = new char [4]; //an int is four bytes
    unsigned short temp; //placeholder for each char
    pf->buffer[0] = b & 255; //b % 256
    b = b >> 8;
    short ib = pf->inode_block;
    short ie = pf->inode_entry; 
    bool add_new_bytes = false;
    INODE_BLOCK i_block;
    block_read(3 + ib, &i_block);
    INODE_ENTRY i_entry = i_block.inodes[ie];
    short num_d_blocks = pf->data_block_cur;
    short loc_data = pf->loc_data_in_block; //can be at maximum 32*20 = 640
    if (i_block == i_entry.data_blocks_used){
        //e.g. i_block is 6 but only 6 blocks are used
        add_new_bytes = true;
    }
    DIR_BLOCK d_block;
    block_read(i_entry.data_blocks[i_block], &d_block);
    short d_e = loc_data & (loc_data & 32); //DIR_ENTRY within DIR_BLOCK
    short d_r = loc_data & 32; //location within dir_entry
    if (!add_new_bytes && d_e == d_block.num_directory_entries) {
        //e.g. d_e is 6 but only 6 entries are used
        add_new_bytes = true;
    }
    DIR_ENTRY d_entry = d_block.director_entries[d_e];
    if (!add_new_bytes && d_r == d_entry.d_reclen) {
        add_new_bytes = true;
    }
    //update PDOS_FILE*
    //32*20 = 640 bytes of data within each d_block
    if (add_new_bytes) {
    }
    pf->loc_data_in_block++;
    if (pf->loc_data_in_block == 640) {
        pf->loc_data_in_block = 0;
        pf->data_block_cur++; //moving onto new data block
    }
}

void pdos_fclose(PDOS_FILE *) {
}

void pdos_mkdir(char *dir) {
}
