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
    bool wantR = strchr(mode,'r')||strchr(mode,'R');
    bool wantW = strchr(mode,'w')||strchr(mode,'W');

    int loc_file = dir_lookup(fname);
    if (loc_file == -1) {
        loc_file = inode_allocate();
        if (loc_file < 0) return NULL;
        INODE_ENTRY newie = {0};
        newie.size = 0;
        newie.data_blocks_used = 0;
        for (int i = 0; i < MAX_DATA_BLOCKS; ++i) newie.data_blocks[i] = 0xFFFF;
        inode_write(loc_file, &newie);
        dir_add(fname, loc_file, FILE_TYPE);

    } else if (wantW && !wantR) {
        // truncate file if opened write-only
        INODE_ENTRY trun = {0};
        trun.size = 0;
        trun.data_blocks_used = 0;
        for (int i = 0; i < MAX_DATA_BLOCKS; ++i) trun.data_blocks[i] = 0xFFFF;
        inode_write(loc_file, &trun);
    }

    int div   = loc_file / INODE_ENTRIES_PER_INODE_BLOCK;
    int block = 3 + div;
    int idx   = loc_file % INODE_ENTRIES_PER_INODE_BLOCK;

    PDOS_FILE *pf = malloc(sizeof(*pf));
    if (!pf) return NULL;
    pf->inode_block       = block;
    pf->inode_index       = idx;
    pf->data_block_cur    = 0;
    pf->loc_data_in_block = 0;
    memset(pf->buffer, 0, sizeof(pf->buffer));
    pf->modeR = wantR;
    pf->modeW = wantW;
    pf->modeX = strchr(mode,'x')||strchr(mode,'X');
    return pf;
}

int pdos_fgetc(PDOS_FILE *pf) {
    if (!pf || !pf->modeR) return EOF;
    INODE_BLOCK iblk;
    block_read(pf->inode_block, &iblk);
    INODE_ENTRY *ie = &iblk.inodes[pf->inode_index];
    int absolute = pf->data_block_cur * BLOCK_SIZE + pf->loc_data_in_block;
    if (absolute >= ie->size) return EOF;
    DATA_BLOCK db;
    block_read(ie->data_blocks[pf->data_block_cur], &db);
    unsigned char c = (unsigned char)db.data[pf->loc_data_in_block++];
    if (pf->loc_data_in_block == BLOCK_SIZE) {
        pf->loc_data_in_block = 0;
        pf->data_block_cur++;
    }
    return c;
}

void pdos_fputc(int b, PDOS_FILE *pf) {
    if (!pf || !pf->modeW) return;
    INODE_BLOCK iblk;
    block_read(pf->inode_block, &iblk);
    INODE_ENTRY *ie = &iblk.inodes[pf->inode_index];
    if (ie->data_blocks[pf->data_block_cur] == 0xFFFF) {
        int newblk = data_block_allocate();
        if (newblk < 0) return;
        ie->data_blocks[pf->data_block_cur] = newblk;
        ie->data_blocks_used++;
    }
    DATA_BLOCK db;
    block_read(ie->data_blocks[pf->data_block_cur], &db);
    db.data[pf->loc_data_in_block] = (char)b;
    block_write(ie->data_blocks[pf->data_block_cur], &db);
    int absolute = pf->data_block_cur * BLOCK_SIZE + pf->loc_data_in_block;
    if (absolute >= ie->size) ie->size = absolute + 1;
    iblk.inodes[pf->inode_index] = *ie;
    block_write(pf->inode_block, &iblk);
    pf->loc_data_in_block++;
    if (pf->loc_data_in_block == BLOCK_SIZE) {
        pf->loc_data_in_block = 0;
        pf->data_block_cur++;
    }
}

void pdos_fclose(PDOS_FILE *pf) {
    if (pf) free(pf);
}

int pdos_mkdir(const char *dirname) {
    int loc_file = inode_allocate();
    if (loc_file < 0) return -1;
    dir_add(dirname, loc_file, DIR_TYPE);

    INODE_ENTRY ien = {0};
    ien.size = 2 * sizeof(DIR_ENTRY);
    ien.data_blocks_used = 1;
    int db = data_block_allocate();
    if (db < 0) return -1;
    ien.data_blocks[0] = db;
    inode_write(loc_file, &ien);

    DIR_BLOCK dblk = { .num_dir_entries = 2 };
    strcpy(dblk.dir_entries[0].d_name, ".");
    dblk.dir_entries[0].d_ino  = loc_file;
    dblk.dir_entries[0].d_type = DIR_TYPE;
    strcpy(dblk.dir_entries[1].d_name, "..");
    dblk.dir_entries[1].d_ino  = 0;
    dblk.dir_entries[1].d_type = DIR_TYPE;
    block_write(db, &dblk);

    return 0;
}

int pdos_read(PDOS_FILE *pf, char *buf, size_t len) {
    if (!pf || !pf->modeR) return -1;
    size_t i;
    for (i = 0; i < len; ++i) {
        int c = pdos_fgetc(pf);
        if (c == EOF) break;
        buf[i] = (char)c;
    }
    return (int)i;
}

int pdos_write(PDOS_FILE *pf, const char *buf, size_t len) {
    if (!pf || !pf->modeW) return -1;
    for (size_t i = 0; i < len; ++i) pdos_fputc(buf[i], pf);
    return (int)len;
}

bool pdos_exists(const char *fname) {
    return dir_lookup(fname) != -1;
}
