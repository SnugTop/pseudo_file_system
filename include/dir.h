#ifndef DIR_H
#define DIR_H

#define MAX_NAME_LENGTH 12

typedef struct {
    char d_name[MAX_NAME_LENGTH];  // File or directory name -- size 12 bytes
    unsigned short d_ino;          // Inode number -- size 2 bytes
    short d_off = 0;                   // Offset to next entry (optional) -- size 2 bytes
    unsigned short d_reclen;         // Size of data -- size 2 bytes
    unsigned char d_type;          // 1 = directory, 2 = file -- size 1 byte
    //bool char d_type;            // 0 = directory, 1 = file
    //total size of 51, allocate 32 bytes to file info
    char data [32]; // size 32 bytes
    //has room for 2 d_name if directory
} DIR_ENTRY;

#define MAX_DIR_ENTRIES ((1024 - size(int)) / sizeof(DIR_ENTRY))
//since 1024 size of DIR_BLOCK total; size(int) used for num_director_entries
//thus, MAX_DIR_ENTRIES = 1020 / sizeof(DIR_ENTRY) = 20

//data block
typedef struct {
    int num_director_entries;              // Number of entries used
    DIR_ENTRY director_entries[MAX_DIR_ENTRIES]; // Directory entries
} DIR_BLOCK;

// Function prototypes
int dir_lookup(const char *filename);
int dir_add(const char *filename, int inode_number, int type);
char **pdos_dir(void);

#endif
