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
 * Parameters:
 * - block_number: which block to read (0 to 1090)
 * - buffer: where to store the data
 * Returns 0 on success, -1 on invalid block number.
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
 * Parameters:
 * - block_number: which block to write (0 to 1090)
 * - buffer: data to write
 * Returns 0 on success, -1 on invalid block number.
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
 * pdos_mkfs:
 * Formats the disk to create a new, empty file system.
 * Writes the superblock, inode bitmap, data bitmap, root directory inode, and root directory block.
 * Parameter:
 * - id: Identifying string to store in superblock.
 * Returns 0 on success.
 */
int pdos_mkfs(char *id) {
    // Step 1: Create the superblock (Block 0)
    char superblock[BLOCK_SIZE];
    memset(superblock, 0, sizeof(superblock));
    strncpy(superblock, id, BLOCK_SIZE);
    block_write(0, superblock);

    // Step 2: Create and initialize the inode bitmap (Block 1)
    unsigned short inode_bitmap[BLOCK_SIZE / 2] = {0}; 
    inode_bitmap[0] = 0x0001; // Mark inode 0 (root directory) as used
    block_write(1, inode_bitmap);

    // Step 3: Create and initialize the data bitmap (Block 2)
    unsigned short data_bitmap[BLOCK_SIZE / 2] = {0};
    data_bitmap[0] = 0x0001; // Mark data block 0 (root dir block) as used
    block_write(2, data_bitmap);

    // Step 4: Create inode 0 (for root directory)
    INODE_BLOCK inode_block;
    memset(&inode_block, 0, sizeof(INODE_BLOCK));

    inode_block.inodes[0].size = 2 * sizeof(DIR_ENTRY); // Two entries: "." and ".."
    inode_block.inodes[0].file_mod_time = time(NULL);   // Set modification time
    inode_block.inodes[0].data_blocks_used = 1;         // Root directory uses 1 data block
    inode_block.inodes[0].data_blocks[0] = 0;           // Points to block 0 (data block)

    block_write(3, &inode_block);

    // Step 5: Create the root directory block (stored at block 67)
    DIR_BLOCK root_dir;
    memset(&root_dir, 0, sizeof(DIR_BLOCK));
    root_dir.num_director_entries = 2; // . and ..

    // Entry for "."
    strcpy(root_dir.director_entries[0].d_name, ".");
    root_dir.director_entries[0].d_ino = 0;
    root_dir.director_entries[0].d_type = DIR_TYPE;

    // Entry for ".."
    strcpy(root_dir.director_entries[1].d_name, "..");
    root_dir.director_entries[1].d_ino = 0;
    root_dir.director_entries[1].d_type = DIR_TYPE;

    block_write(67, &root_dir);

    return 0;
}
