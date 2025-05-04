//disk.c
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

// Global pointer to the base address of the pseudo-disk
void *disk_base = NULL;

/*
 * pdos_mkdisk:
 * Creates and maps a shared memory segment to act as our disk.
 * Unlinks any previous disk with the same name to avoid conflicts.
 * Returns 0 on success, -1 on error.
 */
int pdos_mkdisk(void) {
    // Always unlink any leftover disk from a previous run
    shm_unlink(DISK_NAME);

    // Create and open a shared memory object
    int fd = shm_open(DISK_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open failed");
        return -1;
    }

    // Set the size of the shared memory object
    if (ftruncate(fd, DISK_SIZE) == -1) {
        perror("ftruncate failed");
        close(fd);
        return -1;
    }

    // Map the shared memory object into the process's address space
    disk_base = mmap(NULL, DISK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (disk_base == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return -1;
    }

    close(fd); // File descriptor no longer needed after mmap
    return 0;
}

/*
 * block_read:
 * Reads a 1024-byte block from the pseudo-disk into a buffer.
 */
int block_read(int block_number, void *buffer) {
    if (block_number < 0 || block_number >= TOTAL_BLOCKS) {
        fprintf(stderr, "Invalid block number in read: %d\n", block_number);
        return -1;
    }

    memcpy(buffer, disk_base + (block_number * BLOCK_SIZE), BLOCK_SIZE);
    return 0;
}

/*
 * block_write:
 * Writes a 1024-byte buffer into a specific block on the pseudo-disk.
 */
int block_write(int block_number, const void *buffer) {
    if (block_number < 0 || block_number >= TOTAL_BLOCKS) {
        fprintf(stderr, "Invalid block number in write: %d\n", block_number);
        return -1;
    }

    memcpy(disk_base + (block_number * BLOCK_SIZE), buffer, BLOCK_SIZE);
    return 0;
}

/*
 * data_block_allocate:
 * Finds the first available data block and marks it as used.
 * Returns the block number (relative to data blocks) or -1 if none available.
 */
int data_block_allocate(void) {
    unsigned short bitmap[BLOCK_SIZE / 2]; // 512 words (1024 bytes)
    block_read(2, bitmap); // Read the data bitmap (Block 2)

    for (int word = 0; word < 512; ++word) {
        if (bitmap[word] != 0xFFFF) {
            for (int bit = 0; bit < 16; ++bit) {
                if ((bitmap[word] & (1 << bit)) == 0) {
                    bitmap[word] |= (1 << bit);
                    block_write(2, bitmap);
                    return (word * 16) + bit;
                }
            }
        }
    }

    return -1; // No free blocks
}

/*
 * pdos_mkfs:
 * Formats the disk to create a new, empty file system.
 */
int pdos_mkfs(char *id) {
    // Step 1: Superblock
    char superblock[BLOCK_SIZE];
    memset(superblock, 0, sizeof(superblock));
    strncpy(superblock, id, BLOCK_SIZE);
    block_write(0, superblock);

    // Step 2: Inode bitmap
    unsigned short inode_bitmap[BLOCK_SIZE / 2] = {0}; 
    inode_bitmap[0] = 0x0001; // Mark inode 0 (root directory) as used
    block_write(1, inode_bitmap);

    // Step 3: Data bitmap
    unsigned short data_bitmap[BLOCK_SIZE / 2] = {0};
    data_bitmap[0] = 0x0001; // Mark data block 0 (used by root dir)
    block_write(2, data_bitmap);

    // Step 4: Root inode
    INODE_BLOCK inode_block;
    memset(&inode_block, 0, sizeof(INODE_BLOCK));
    inode_block.inodes[0].size = 2 * sizeof(DIR_ENTRY);
    inode_block.inodes[0].file_mod_time = time(NULL);
    inode_block.inodes[0].data_blocks_used = 1;
    inode_block.inodes[0].data_blocks[0] = 0;
    block_write(3, &inode_block);

    // Step 5: Root directory entries
    DIR_BLOCK root_dir;
    memset(&root_dir, 0, sizeof(DIR_BLOCK));
    root_dir.num_dir_entries = 2;

    strcpy(root_dir.dir_entries[0].d_name, ".");
    root_dir.dir_entries[0].d_ino = 0;
    root_dir.dir_entries[0].d_type = DIR_TYPE;

    strcpy(root_dir.dir_entries[1].d_name, "..");
    root_dir.dir_entries[1].d_ino = 0;
    root_dir.dir_entries[1].d_type = DIR_TYPE;

    block_write(67, &root_dir);

    return 0;
}
