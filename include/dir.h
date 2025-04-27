#ifndef DIR_H
#define DIR_H

#define MAX_NAME_LENGTH 12

typedef struct direntry {
    char d_name[MAX_NAME_LENGTH];   // ascii name
    unsigned short d_ino;           // inode number
    short d_off;                    // offset to next dir entry (optional)
    unsigned short d_reclen;        // not really used
    unsigned char d_type;           // 1 = dir, 2 = file
} DIR_ENTRY;

#define MAX_DIR_ENTRIES (1024 / sizeof(DIR_ENTRY))

typedef struct DIR_BLOCK {
    int num_director_entries;                     // number of entries used
    DIR_ENTRY director_entries[MAX_DIR_ENTRIES];   // entries
} DIR_BLOCK;

#endif
