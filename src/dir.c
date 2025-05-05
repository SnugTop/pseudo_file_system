// dir.c
#include "dir.h"
#include "defs.h"
#include "disk.h"
#include "inode.h"
#include <string.h>
#include <stdlib.h>

/*
 * dir_lookup:
 *   Searches the root directory’s data_blocks for a file by name.
 *   Returns the inode number if found, -1 if not found.
 */
int dir_lookup(const char *filename) {
    INODE_ENTRY root_inode;
    if (inode_read(0, &root_inode) < 0) return -1;

    DIR_BLOCK db;
    for (int bi = 0; bi < root_inode.data_blocks_used; ++bi) {
        int blk = root_inode.data_blocks[bi];
        block_read(blk, &db);
        for (int i = 0; i < db.num_dir_entries; ++i) {
            if (strcmp(db.dir_entries[i].d_name, filename) == 0) {
                return db.dir_entries[i].d_ino;
            }
        }
    }
    return -1;
}

/*
 * dir_add:
 *   Adds a new entry to the root directory.  If the last block is full,
 *   allocates a fresh data–block and appends to root_inode.data_blocks[].
 */
int dir_add(const char *filename, int inode_number, int type) {
    INODE_ENTRY root_inode;
    inode_read(0, &root_inode);

    DIR_BLOCK db;
    int blk, i;
    // find a block with space
    for (int bi = 0; bi < root_inode.data_blocks_used; ++bi) {
        blk = root_inode.data_blocks[bi];
        block_read(blk, &db);
        if (db.num_dir_entries < MAX_DIR_ENTRIES) {
            goto have_space;
        }
    }
    // no space: allocate new block
    {
        int newblk = data_block_allocate();
        if (newblk < 0) return -1;
        root_inode.data_blocks[root_inode.data_blocks_used++] = newblk;
        inode_write(0, &root_inode);
        memset(&db, 0, sizeof(db));
        blk = newblk;
    }
have_space:
    // insert at db.num_dir_entries
    i = db.num_dir_entries++;
    strncpy(db.dir_entries[i].d_name, filename, MAX_NAME_LENGTH);
    db.dir_entries[i].d_ino    = inode_number;
    db.dir_entries[i].d_type   = type;
    db.dir_entries[i].d_reclen = sizeof(DIR_ENTRY);
    db.dir_entries[i].d_off    = 0;
    block_write(blk, &db);
    return 0;
}

/*
 * pdos_dir:
 *   Returns a NULL‐terminated array of strings for the root directory.
 *   Caller frees each string + the array.
 */
char **pdos_dir(void) {
    INODE_ENTRY root_inode;
    if (inode_read(0, &root_inode) < 0) return NULL;

    DIR_BLOCK db;
    int total = 0;
    // count entries
    for (int bi = 0; bi < root_inode.data_blocks_used; ++bi) {
        block_read(root_inode.data_blocks[bi], &db);
        total += db.num_dir_entries;
    }

    char **list = malloc((total + 1) * sizeof(char *));
    if (!list) return NULL;

    int idx = 0;
    for (int bi = 0; bi < root_inode.data_blocks_used; ++bi) {
        block_read(root_inode.data_blocks[bi], &db);
        for (int i = 0; i < db.num_dir_entries; ++i) {
            list[idx++] = strdup(db.dir_entries[i].d_name);
        }
    }
    list[idx] = NULL;
    return list;
}
