#ifndef FS_H
#define FS_H

#include "dir.h"    // for MAX_NAME_LENGTH
#include "inode.h"
#include "disk.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

// Maximum number of data blocks per file
#define MAX_DATA_BYTES 1017  // (15 * 1024) − overhead

typedef struct {
    unsigned short inode_block;       // block number of the inode
    unsigned short inode_index;       // index within that inode block
    unsigned short data_block_cur;    // which data block we’re reading/writing
    unsigned short loc_data_in_block; // offset within the current data block
    char           buffer[1024];      // I/O buffer
    bool           modeW, modeX, modeR;
} PDOS_FILE;

PDOS_FILE *pdos_open(const char *fname, const char *mode);
int         pdos_fgetc(PDOS_FILE *pf);          // <-- Add this
void        pdos_fputc(int b, PDOS_FILE *pf);   // <-- And this
void        pdos_fclose(PDOS_FILE *pf);
int         pdos_read(PDOS_FILE *pf, char *buf, size_t len);
int         pdos_write(PDOS_FILE *pf, const char *buf, size_t len);
bool        pdos_exists(const char *fname);
int         pdos_mkdir(const char *dirname);

#endif
