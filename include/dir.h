#ifndef DIR_H
#define DIR_H

#define MAX_NAME_LENGTH 12

typedef struct {
    char d_name[MAX_NAME_LENGTH];  // File or directory name -- size 12 bytes
    unsigned short d_ino;          // Inode number -- size 2 bytes
    short d_off = 0;                   // Offset to next entry (optional) -- size 2 bytes
    unsigned short d_reclen;         // Size of data -- size 2 bytes
    unsigned char d_type;          // 1 = directory, 2 = file -- size 2 bytes
} DIR_ENTRY;

#define MAX_DIR_ENTRIES 1024

//data block
typedef struct {
    char data [MAX_DIR_ENTRIES]; // Directory entries
} DATA_BLOCK;

typedef struct {
    int dir_entries; //size 4 bytes
    DIR_ENTRY dir_entries [51]; //1024 minus 4 divided by 20 is 51
} DIR_BLOCK;

// Function prototypes
int dir_lookup(const char *filename);
int dir_add(const char *filename, int inode_number, int type);
char **pdos_dir(void);

#endif
