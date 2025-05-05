//fs.c
#include "dir.h"
#include "defs.h"
#include "disk.h"
#include "inode.h"
#include "fs.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/*
 * pdos_open:
 *   mode:
 *     "r"  => read-only
 *     "w"  => create or truncate
 *     "rw" => read+write
 */
PDOS_FILE *pdos_open(const char *fname, const char *mode) {
    // mode is char* array; check for "r", "w", "x"
    bool wantR = strchr(mode, 'r') || strchr(mode, 'R');
    bool wantW = strchr(mode, 'w') || strchr(mode, 'W');
    bool wantX = strchr(mode, 'x') || strchr(mode, 'X');

    // looks for where file is stored
    int loc_file = dir_lookup(fname);

    // if new file to be created
    if (loc_file == -1) {
        // looks for first free inode
        loc_file = inode_allocate();
        if (loc_file < 0) return NULL;

        // allocate data blocks and stuff for new Inode
        INODE_ENTRY i_e = {0};
        i_e.data_blocks_used = 0;
        i_e.size = 0;
        for (int count = 0; count < MAX_DATA_BLOCKS; count++) {
            i_e.data_blocks[count] = 0xFFFF; // mark as unused
        }

        inode_write(loc_file, &i_e);
        // new file added in
        dir_add(fname, loc_file, FILE_TYPE); 
    } else if (wantW && !wantR) {
        // truncate if opened write-only
        INODE_ENTRY trunc = {0};
        trunc.size = 0;
        trunc.data_blocks_used = 0;
        for (int i = 0; i < MAX_DATA_BLOCKS; ++i) {
            trunc.data_blocks[i] = 0xFFFF;
        }
        inode_write(loc_file, &trunc);
    }

    // if file already exists then loc_file == inode number
    int div = loc_file / 16;  // (loc_file)/16
    int block = 3 + div;      // which block it's in
    int loc = loc_file % 16;  // which inode within the block

    // allocate and fill PDOS_FILE structure
    PDOS_FILE *pf = malloc(sizeof(PDOS_FILE));
    if (!pf) return NULL;

    pf->inode_block = block;            // starting at 3 to account for prior blocks
    pf->inode_index = loc;
    pf->data_block_cur = 0;
    pf->loc_data_in_block = 0;
    memset(pf->buffer, 0, sizeof(pf->buffer));

    pf->modeR = wantR;
    pf->modeW = wantW;
    pf->modeX = wantX;

    return pf;
}

int pdos_fgetc(PDOS_FILE *pf) {
    if (!pf || pf->modeR == 0) {
        return EOF; // don't want to throw an error just for this
    }

    unsigned short ib = pf->inode_block;
    unsigned short ie = pf->inode_index; 

    INODE_BLOCK i_block;
    block_read(ib, &i_block);
    INODE_ENTRY *i_entry = &i_block.inodes[ie];

    if (i_entry->size <= pf->data_block_cur * 1024 + pf->loc_data_in_block) {
        // reached or passed EOF
        return EOF;
    }

    DATA_BLOCK d_block;
    block_read(i_entry->data_blocks[pf->data_block_cur], &d_block);

    char c = d_block.data[pf->loc_data_in_block];

    // incrementation
    pf->loc_data_in_block += 1;
    if (pf->loc_data_in_block == 1024) {
        // going to next block
        pf->loc_data_in_block = 0;
        pf->data_block_cur += 1;
    }

    return (unsigned char)c;
}

void pdos_fputc(int b, PDOS_FILE *pf) {
    if (!pf || pf->modeW == 0) return;

    char c = (char)b;

    unsigned short ib = pf->inode_block;
    unsigned short ie = pf->inode_index; 

    INODE_BLOCK i_block;
    block_read(ib, &i_block);
    INODE_ENTRY *i_entry = &i_block.inodes[ie];

    // allocate block if needed
    if (i_entry->data_blocks[pf->data_block_cur] == 0xFFFF) {
        int new_block = data_block_allocate();
        if (new_block < 0) return;
        i_entry->data_blocks[pf->data_block_cur] = new_block;
        i_entry->data_blocks_used++;
    }

    DATA_BLOCK d_block;
    block_read(i_entry->data_blocks[pf->data_block_cur], &d_block);

    // write byte
    d_block.data[pf->loc_data_in_block] = c;
    block_write(i_entry->data_blocks[pf->data_block_cur], &d_block);

    // update file size
    int absolute_pos = pf->data_block_cur * 1024 + pf->loc_data_in_block;
    if (absolute_pos >= i_entry->size) {
        i_entry->size = absolute_pos + 1;
    }

    // save inode update
    i_block.inodes[ie] = *i_entry;
    block_write(ib, &i_block);

    // incrementation
    pf->loc_data_in_block += 1;
    if (pf->loc_data_in_block == 1024) {
        pf->loc_data_in_block = 0;
        pf->data_block_cur += 1;
    }
}

void pdos_fclose(PDOS_FILE *pf) {
    if (pf) {
        free(pf); // not deallocating memory blocks, just removing the pointer
    }
}

int pdos_mkdir(const char *dirname) {
    // looks for first free inode
    int loc_file = inode_allocate();
    if (loc_file < 0) return -1;

    dir_add(dirname, loc_file, DIR_TYPE); // new directory added in

    INODE_ENTRY i_entry = {0};
    i_entry.size = 2 * sizeof(DIR_ENTRY);
    i_entry.data_blocks_used = 1;

    int new_block = data_block_allocate();
    if (new_block < 0) return -1;

    i_entry.data_blocks[0] = new_block;
    for (int i = 1; i < MAX_DATA_BLOCKS; ++i)
        i_entry.data_blocks[i] = 0xFFFF;

    inode_write(loc_file, &i_entry);

    // Create . and .. entries
    DIR_BLOCK root_dir;
    memset(&root_dir, 0, sizeof(DIR_BLOCK));
    root_dir.num_dir_entries = 2;

    strcpy(root_dir.dir_entries[0].d_name, ".");
    root_dir.dir_entries[0].d_ino = loc_file;
    root_dir.dir_entries[0].d_type = DIR_TYPE;

    strcpy(root_dir.dir_entries[1].d_name, "..");
    root_dir.dir_entries[1].d_ino = 0;
    root_dir.dir_entries[1].d_type = DIR_TYPE;

    block_write(new_block, &root_dir);
    return 0;
}

int pdos_read(PDOS_FILE *pf, char *buf, size_t len) {
    if (!pf || !pf->modeR) return -1;

    for (size_t i = 0; i < len; ++i) {
        int c = pdos_fgetc(pf);
        if (c == EOF) return i; // Return bytes successfully read
        buf[i] = (char)c;
    }
    return len;
}

int pdos_write(PDOS_FILE *pf, const char *buf, size_t len) {
    if (!pf || !pf->modeW) return -1;

    for (size_t i = 0; i < len; ++i) {
        pdos_fputc(buf[i], pf);
    }
    return len;
}

bool pdos_exists(const char *fname) {
    return dir_lookup(fname) != -1;
}
