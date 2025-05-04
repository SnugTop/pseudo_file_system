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
    unsigned short inode_block;              //corresponding inode -- 16*block + loc, loc being inode index in block
    unsigned short inode_index;
    unsigned short data_block_cur;       //which data block is the pointer on
    unsigned short loc_data_in_block;    //where I am within that data block
    char* buffer;                   //unspecified size
    bool modeW;                     //write
    bool modeX;                     //execute
    bool modeR;                     //read
} PDOS_FILE;

// Function prototypes
PDOS_FILE *pdos_open(const char *fname, const char *mode);
int pdos_fgetc(PDOS_FILE * pf);
void pdos_fputc(int b, PDOS_FILE * pf);
void pdos_fclose(PDOS_FILE * pf);
void pdos_mkdir(char *dir);

#endif
