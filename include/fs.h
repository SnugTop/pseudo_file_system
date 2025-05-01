#ifndef FS_H
#define FS_H

#include dir.h //for MAX_NAME_LENGTH
//since we have max 15 data blocks and 1024 bytes per data block
#define MAX_DATA_BYTES = 1017
#include inode.h
#include disk.h
#include dir.h

//I have no clue if this is your plan for the file structure, but as opposed to a directory...

typedef struct {
    short inode_block;              //corresponding inode -- 16*block + loc, loc being inode index in block
    short inode_index;
    short data_block_cur = 0;       //which data block is the pointer on
    short loc_data_in_block = 0;    //where I am within that data block
    char* buffer;                   //unspecified size
} PDOS_FILE;

#endif
