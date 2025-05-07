#ifndef FS_H
#define FS_H

#include <stdbool.h>
#include <stddef.h>
#include "dir.h"
#include "inode.h"
#include "disk.h"

// File‐handle type
typedef struct {
    unsigned short inode_block;
    unsigned short inode_index;
    unsigned short data_block_cur;
    unsigned short loc_data_in_block;
    char           buffer[BLOCK_SIZE];
    bool           modeW, modeX, modeR;
} PDOS_FILE;

// Core API
PDOS_FILE *pdos_open(const char *fname, const char *mode);
int        pdos_fgetc(PDOS_FILE *pf);
void       pdos_fputc(int b, PDOS_FILE *pf);
void       pdos_fclose(PDOS_FILE *pf);
int        pdos_read(PDOS_FILE *pf, char *buf, size_t len);
int        pdos_write(PDOS_FILE *pf, const char *buf, size_t len);
bool       pdos_exists(const char *fname);

void       pdos_mkdir(char *dirname);

#endif 
