#ifndef DIR_H
#define DIR_H

#define MAX_NAME_LENGTH 12

typedef struct direntry {
    char d_name[MAX_NAME_LENGTH];  // File or directory name
    unsigned short d_ino;          // Inode number
    short d_off;                   // Offset to next entry (optional)
    unsigned short d_reclen;       // Not really used
    unsigned char d_type;          // 1 = directory, 2 = file
} DIR_ENTRY;

#define MAX_DIR_ENTRIES (1024 / sizeof(DIR_ENTRY))

typedef struct DIR_BLOCK {
    int num_director_entries;              // Number of entries used
    DIR_ENTRY director_entries[MAX_DIR_ENTRIES]; // Directory entries
} DIR_BLOCK;

// Function prototypes
int dir_lookup(const char *filename);
int dir_add(const char *filename, int inode_number, int type);
char **pdos_dir(void);

#endif
