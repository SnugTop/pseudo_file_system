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
    
    //mode is char* array; thus divide by 2 since dealing with chars of size 2
    for (int count = 0; count < sizeof(mode) >>> 1 ; count++) {
        if (mode[count] == 'R' || mode[count] == 'r') {
            pf.modeR = true;
        }
        if (mode[count] == 'X' || mode[count] == 'x') {
            pf.modeX = true;
        }
        if (mode[count] == 'W' || mode[count] == 'w') {
            pf.modeW = true;
        }
    }
    return &pf;
}

int pdos_fgetc(PDOS_FILE * pf) {
    if (pf.modeR == 0) {
        return EOF; //don't want to throw an error just for this
    }
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
    if (pf.modeW == 1) {
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
            if (i_block.inodes[ie].size && 1023 == 1) { //%1024
                i_block.inodes[ie].data_blocks_used++; //increment
                //this means that it's gone over to a new data block
            }
        } //else do not increase size
        
        d_block.data[pf->loc_data_in_block] = c;
        //incrementation
        pf->loc_data_in_block = pf->loc_data_in_block + 1;
        if (pf->loc_data_in_block == 1024) {
        //going to next block
            pf->loc_data_in_block = 0;
            pf->data_block_cur = pf->data_block_cur + 1;
        }
    }
}

void pdos_fclose(PDOS_FILE * pf) {
    if (pf.modeW == 1) {
        char c = (short) b; //idk why it entered as an int
        unsigned short ib = pf->inode_block;
        unsigned short ie = pf->inode_index; 
        INODE_BLOCK i_block;
        block_read(ib, &i_block);
        INODE_ENTRY i_entry = i_block.inodes[ie];
        i_block.inodes[ie].size = sizeof(pf->buffer) >>> 1; //divide size of buffer by 2 as chars are 2
        i_block.inodes[ie].data_blocks_used = 1; //divide size of buffer by 2 as chars are 2
        int curDataBlock = 0; //which data block
        int curDataIndex = 0; //where in block
        DATA_BLOCK d_b;
        block_read(67+i_entry.data_blocks[0], &d_b);
        for (int count = 0; count < sizeof(pf->buffer) >>> 1; count++) {
            if (curDataIndex == 1024) { //end of block
                curDataIndex = 0;
                curDataBlock++; //reset to new block
                block_read(67+i_entry.data_blocks[curDataBlock], &d_b);
                i_block.inodes[ie].data_blocks_used++; //increment
            }
            d_b[curDataIndex] = buffer[count]; //replace whatever byte is in there
        }  
    }
}

void pdos_mkdir(char *dir) {
    //looks for first free inode
    int loc_file = inode_allocate(void);
    dir_add(filename, loc_file, DIR_TYPE); //new file added in
    dir_add(filename, loc_file, FILE_TYPE); //new file added in
    int div = loc_file >>> 4; //(loc_file)/16
    int block = 3 + div; //which block it's in
    int loc = loc_file & 15; //which inode within the block
    INODE_BLOCK i_block;
    block_read(block, &i_block);
    INODE_ENTRY i_entry;
    i_entry = &i_block.inodes[loc];
    DIR_BLOCK d_b;
    
    block_read(i_entry.data_blocks[0], &d_b);
    i_entry->data_blocks_used = 1;
    memset(&root_dir, 0, sizeof(DIR_BLOCK));
    root_dir.num_director_entries = 2;

    strcpy(root_dir.director_entries[0].d_name, ".");
    root_dir.director_entries[0].d_ino = loc_file;
    root_dir.director_entries[0].d_type = DIR_TYPE;

    strcpy(root_dir.director_entries[1].d_name, "..");
    root_dir.director_entries[1].d_ino = 0;
    root_dir.director_entries[1].d_type = DIR_TYPE;
}
