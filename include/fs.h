#ifndef FS_H
#define FS_H

#include dir.h //for MAX_NAME_LENGTH
//since we have 1024 data blocks and 1024 files, we assume max file size is 1024 bytes i.e. data block size
#define MAX_DATA_BYTES = 1017

//I have no clue if this is your plan for the file structure, but as opposed to a directory...

typedef struct {
    char d_name[MAX_NAME_LENGTH];  // File or directory name -- size 12 bytes
    short d_off = 0;                   // Location of pointer, initially 0 and max 1023 -- size 2 bytes
    short data_size = 0;          // How many bytes of data -- size 2 bytes
    char data[MAX_DATA_BYTES];    // Actual file contents -- size 1018 bytes
} FILE_BLOCK;

#endif
