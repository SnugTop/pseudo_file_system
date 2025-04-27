// File: pseudoFileSystem.h
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

#ifndef PSEUDO_FILE_SYSTEM_H
#define PSEUDO_FILE_SYSTEM_H

#define BLOCK_SIZE 1024
#define MAX_BLOCKS 1091 // Total blocks in our file system

// Raw Data Block
typedef struct DISK_BLOCK {
    char DATA_BLOCK[BLOCK_SIZE];
} DISK_BLOCK;

// File 
#define MAX_NAME_LENGTH     12
#define MAX_PATH_LENGTH     512

#define INODE_ENTRIES_PER_INODE_BLOCK 16

// MAX NUMBER OF DIRECT POINTERS A FILE CAN HAVE FOR EXPANSION; 
#define MAX_DATA_BLOCKS 15 

//const unsigned int FILE_MAX_DATA_BLOCKS = MAX_BLOCKS - SUPER_BLOCK - IBITMAP_BLOCK + DBITMAP; 
// each directoy entry contains the following
//
typedef struct direntry{
    char            d_name[MAX_NAME_LENGTH]; // ascii name
    unsigned short  d_ino;      // inode nuber for this entry
    short  d_off;               // offset to next directory entry, convenience  
    unsigned short  d_reclen;   // not really used. length of this record
    unsigned char   d_type; // entry type (directory = 1, file = 2)
} DIR_ENTRY;

//const int MAX_DIR_ENTRIES = BLOCK_SIZE / sizeof(DIR_ENTRY);
#define MAX_DIR_ENTRIES ( BLOCK_SIZE / sizeof(DIR_ENTRY) )

// Contents of a directory block
typedef struct DIR_BLOCK{
    int num_director_entries; // number of entries used
    DIR_ENTRY director_entries[MAX_DIR_ENTRIES]; // list of directory entries 
} DIR_BLOCK;

//
// inode-entry
// structure of meta-data for each inode that is stored in the
// a inode-block
typedef struct INODE_ENTRY{
    unsigned int    size;  // total size of file
    time_t          file_mod_time;  // file modification time
    unsigned short  data_blocks_used; // total blocks used for this item
    unsigned short  data_blocks[MAX_DATA_BLOCKS];
    unsigned char   fill[16];  // no useful information, used for alignment
}INODE_ENTRY;

// INODE_BLOCK stores inode-entries
// each inode-entry stores meta-data for files or a directory
// my INODE_BLOCK holds 16 inode-entries [0-15]
typedef struct INODE_BLOCK{
    INODE_ENTRY inodes[INODE_ENTRIES_PER_INODE_BLOCK];
}INODE_BLOCK;

/* 
 * File Descriptor --
 *		similar to file descriptor in LINUX
 *		used to store information about current opened
 *		file
 */
typedef struct FILE_DESCRIPTOR{
	char 		mode[3];	// "r", "w" or "rw"
	int 		pos;		// position in current block (byte location of next read/write in block)
	short		blocknum;	// Number of the block being used
    unsigned short d_ino;   // inode number for this file
    DISK_BLOCK      buffer; // used to read/write. We write a buffer to disk when full
                            // or read a new disk-block if we read past current buffer contents
} PDOS_FILE;

// used to manage available data-blocks
typedef struct DATA_BITMASK_BLOCK{
    unsigned short data_blocks[512];
} DATA_BITMASK_BLOCK;

/* 
    function prototypes for library
*/
int pdos_fgetc(PDOS_FILE*);
int pdos_fputc(int, PDOS_FILE*);
// etc....

#endif