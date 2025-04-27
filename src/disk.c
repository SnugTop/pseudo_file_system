//disk.c
#include "defs.h"
#include "disk.h"
#include "inode.h"
#include "dir.h"

#include <fcntl.h>      
#include <sys/mman.h>   
#include <sys/stat.h>   
#include <unistd.h>     
#include <string.h>     
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>       

void *disk_base = NULL;

int pdos_mkdisk(void) {
    int fd = shm_open(DISK_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open failed");
        return -1;
    }

    if (ftruncate(fd, DISK_SIZE) == -1) {
        perror("ftruncate failed");
        close(fd);
        return -1;
    }

    disk_base = mmap(NULL, DISK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (disk_base == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return -1;
    }

    close(fd); 
    return 0;
}

int block_read(int block_number, void *buffer) {
    if (block_number < 0 || block_number >= TOTAL_BLOCKS) {
        fprintf(stderr, "Invalid block number in read: %d\n", block_number);
        return -1;
    }

    memcpy(buffer, disk_base + (block_number * BLOCK_SIZE), BLOCK_SIZE);
    return 0;
}

int block_write(int block_number, const void *buffer) {
    if (block_number < 0 || block_number >= TOTAL_BLOCKS) {
        fprintf(stderr, "Invalid block number in write: %d\n", block_number);
        return -1;
    }

    memcpy(disk_base + (block_number * BLOCK_SIZE), buffer, BLOCK_SIZE);
    return 0;
}

int pdos_mkfs(char *id) {
    // Step 1: Superblock
    char superblock[BLOCK_SIZE];
    memset(superblock, 0, sizeof(superblock));
    strncpy(superblock, id, BLOCK_SIZE);
    block_write(0, superblock);

    // Step 2: Inode bitmap
    unsigned short inode_bitmap[BLOCK_SIZE / 2] = {0}; 
    inode_bitmap[0] = 0x0001; 
    block_write(1, inode_bitmap);

    // Step 3: Data bitmap
    unsigned short data_bitmap[BLOCK_SIZE / 2] = {0};
    data_bitmap[0] = 0x0001;
    block_write(2, data_bitmap);

    // Step 4: Inode block (root dir)
    INODE_BLOCK inode_block;
    memset(&inode_block, 0, sizeof(INODE_BLOCK));

    inode_block.inodes[0].size = 2 * sizeof(DIR_ENTRY);
    inode_block.inodes[0].file_mod_time = time(NULL);
    inode_block.inodes[0].data_blocks_used = 1;
    inode_block.inodes[0].data_blocks[0] = 0; 

    block_write(3, &inode_block);

    // Step 5: Root directory block
    DIR_BLOCK root_dir;
    memset(&root_dir, 0, sizeof(DIR_BLOCK));
    root_dir.num_director_entries = 2;

    strcpy(root_dir.director_entries[0].d_name, ".");
    root_dir.director_entries[0].d_ino = 0;
    root_dir.director_entries[0].d_type = DIR_TYPE;

    strcpy(root_dir.director_entries[1].d_name, "..");
    root_dir.director_entries[1].d_ino = 0;
    root_dir.director_entries[1].d_type = DIR_TYPE;

    block_write(67, &root_dir);

    return 0;
}
