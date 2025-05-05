//fs.h
#ifndef FS_H
#define FS_H

#include "dir.h" //for MAX_NAME_LENGTH
//since we have max 15 data blocks and 1024 bytes per data block
#define MAX_DATA_BYTES 1017
#include "inode.h"
#include "disk.h"
#include "dir.h"
#include <stdbool.h>

//I have no clue if this is your plan for the file structure, but as opposed to a directory...

typedef struct {
    unsigned short inode_block;              //corresponding inode -- 16*block + loc, loc being inode index in block
    unsigned short inode_index;
    unsigned short data_block_cur;       //which data block is the pointer on
    unsigned short loc_data_in_block;    //where I am within that data block
    char buffer[1024];                  //1024 bytes
    bool modeW;                     //write
    bool modeX;                     //execute
    bool modeR;                     //read
} PDOS_FILE; //1035 bytes total

int pdos_read(PDOS_FILE *pf, char *buf, size_t len);
int pdos_write(PDOS_FILE *pf, const char *buf, size_t len);
bool pdos_exists(const char *fname);

#endif
