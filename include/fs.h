#ifndef FS_H
#define FS_H

#include dir.h //for MAX_NAME_LENGTH
//since we have max 15 data blocks and 1024 bytes per data block
#define MAX_DATA_BYTES = 1017

//I have no clue if this is your plan for the file structure, but as opposed to a directory...

typedef struct {
    short inode;                    //corresponding inode
    short d_off = 0;                   // Location of pointer, initially 0
    short data_size = 0;          // How many bytes of data -- size 2 bytes
    unsigned char d_type = 2;
    char* data;    // Actual file contents -- size 1018 bytes
} PDOS_FILE;

#endif
