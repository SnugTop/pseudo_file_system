// disk.c
#include "defs.h"
#include "disk.h"
#include "inode.h"
#include "dir.h"

#include <fcntl.h>      // For shm_open()
#include <sys/mman.h>   // For mmap()
#include <sys/stat.h>   // For mode constants
#include <unistd.h>     // For ftruncate(), close()
#include <string.h>     // For memset(), strncpy(), memcpy()
#include <stdio.h>      // For perror(), fprintf()
#include <stdlib.h>     // For exit()
#include <time.h>       // For time()
#include <stdbool.h>    // For bool

// Global pointer to the base address of the pseudo-disk
void *disk_base = NULL;

/*
 * pdos_mkdisk:
 *   fresh==true  => unlink + truncate + map
 *   fresh==false => just map existing shm
 */
int pdos_mkdisk(bool fresh) {
    if (fresh) {
        shm_unlink(DISK_NAME);
    }
    int fd = shm_open(DISK_NAME, O_CREAT | O_RDWR, 0666);
    if (fd < 0) {
        perror("shm_open failed");
        return -1;
    }
    if (fresh) {
        if (ftruncate(fd, DISK_SIZE) == -1) {
            perror("ftruncate failed");
            close(fd);
            return -1;
        }
    }
    disk_base = mmap(NULL, DISK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (disk_base == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

/*
 * block_read / block_write:
 *   Raw 1024-byte block I/O at block_number [0..TOTAL_BLOCKS-1]
 */
int block_read(int block_number, void *buffer) {
    if (block_number < 0 || block_number >= TOTAL_BLOCKS) {
        fprintf(stderr, "Invalid block read %d\n", block_number);
        return -1;
    }
    memcpy(buffer, (char*)disk_base + block_number*BLOCK_SIZE, BLOCK_SIZE);
    return 0;
}

int block_write(int block_number, const void *buffer) {
    if (block_number < 0 || block_number >= TOTAL_BLOCKS) {
        fprintf(stderr, "Invalid block write %d\n", block_number);
        return -1;
    }
    memcpy((char*)disk_base + block_number*BLOCK_SIZE, buffer, BLOCK_SIZE);
    return 0;
}

/*
 * data_block_allocate:
 *   Finds first free data-block (skipping the one we reserve for root=relative 0).
 *   Returns an *absolute* block number >= 67 on success, or -1 if none.
 */
int data_block_allocate(void) {
    unsigned short bitmap[BLOCK_SIZE/2];
    block_read(2, bitmap);             // read data‐bitmap (block 2)

    // 1024 bits => 512 words
    for (int w = 0; w < BLOCK_SIZE/2; ++w) {
        if (bitmap[w] != 0xFFFF) {
            for (int b = 0; b < 16; ++b) {
                int rel = w*16 + b;
                if (rel == 0) continue;       // never re‐allocate the root dir block
                if (!(bitmap[w] & (1<<b))) {
                    bitmap[w] |= (1<<b);
                    block_write(2, bitmap);
                    return 67 + rel;         // absolute block number
                }
            }
        }
    }
    return -1;
}

/*
 * pdos_mkfs:
 *   Layout:
 *    0 = superblock
 *    1 = inode‐bitmap
 *    2 = data‐bitmap
 *    3 = inode‐block #0  (root inode)
 *    67 = root directory data
 */
int pdos_mkfs(char *id) {
    // 0) superblock
    char sb[BLOCK_SIZE] = {0};
    strncpy(sb, id, BLOCK_SIZE);
    block_write(0, sb);

    // 1) inode‐bitmap: mark inode‐0 used
    unsigned short ibmap[BLOCK_SIZE/2] = {0};
    ibmap[0] = 0x0001;
    block_write(1, ibmap);

    // 2) data‐bitmap: reserve relative‐0 (absolute block 67) for root dir
    unsigned short dbmap[BLOCK_SIZE/2] = {0};
    dbmap[0] = 0x0001;
    block_write(2, dbmap);

    // 3) root inode (inode #0)
    INODE_BLOCK iblk = {0};
    iblk.inodes[0].size = 2 * sizeof(DIR_ENTRY);          // two entries: "." and ".."
    iblk.inodes[0].file_mod_time = time(NULL);
    iblk.inodes[0].data_blocks_used = 1;
    iblk.inodes[0].data_blocks[0] = 67;                   // absolute block for root dir
    block_write(3, &iblk);

    // 3.1) other inode blocks (blocks 4 through 66)
    for (int block = 4; block <= 66; block++) {
        INODE_BLOCK i_blk = {0};
        block_write(block, &i_blk);
    }


    // 4) root directory block @ absolute 67
    DIR_BLOCK droot = { .num_dir_entries = 2 };
    // "."
    strcpy(droot.dir_entries[0].d_name, ".");
    droot.dir_entries[0].d_ino  = 0;
    droot.dir_entries[0].d_type = DIR_TYPE;
    // ".."
    strcpy(droot.dir_entries[1].d_name, "..");
    droot.dir_entries[1].d_ino  = 0;
    droot.dir_entries[1].d_type = DIR_TYPE;

    block_write(67, &droot);

    return 0;
}
