//inode.c
#include "inode.h"
#include "defs.h"
#include "disk.h"
#include <string.h>
#include <stdio.h>

// Inode Bitmap is stored in Block 1
// Each bit represents one inode (0 = free, 1 = used)

/*
 * inode_allocate:
 * Finds the first free inode, marks it as used, and returns its inode number.
 * Returns -1 if no free inodes are available.
 */
int inode_allocate(void) {
    unsigned short bitmap[BLOCK_SIZE / 2]; // 1024 bytes / 2 = 512 unsigned shorts

    // Read the inode bitmap block (block 1)
    block_read(1, bitmap);

    // Scan through each 16-bit word
    for (int word = 0; word < (BLOCK_SIZE / 2); ++word) {
        // If this word isn't fully occupied (not all bits are 1)
        if (bitmap[word] != 0xFFFF) {
            // Scan each bit in the 16-bit word
            for (int bit = 0; bit < 16; ++bit) {
                // If this bit is 0 (free inode)
                if ((bitmap[word] & (1 << bit)) == 0) {
                    // Mark the inode as used (set the bit to 1)
                    bitmap[word] |= (1 << bit);

                    // Write the updated bitmap back to disk
                    block_write(1, bitmap);

                    // Return inode number: word * 16 + bit
                    return (word * 16) + bit;
                }
            }
        }
    }

    // No free inodes found
    return -1;
}

/*
 * inode_read:
 * Reads the inode entry for a given inode number into memory.
 * Parameters:
 * - inode_number: which inode to read
 * - inode: pointer to store the result
 * Returns 0 on success, -1 on invalid inode number.
 */
int inode_read(int inode_number, INODE_ENTRY *inode) {
    // Validate inode number range
    if (inode_number < 0 || inode_number >= 1024) {
        fprintf(stderr, "Invalid inode number: %d\n", inode_number);
        return -1;
    }

    // Calculate which block contains this inode
    // Inodes start at block 3, 16 inodes per block
    int block_number = 3 + (inode_number / 16);

    INODE_BLOCK inode_block;
    block_read(block_number, &inode_block);

    // Copy the specific inode into the provided structure
    *inode = inode_block.inodes[inode_number % 16];

    return 0;
}

/*
 * inode_write:
 * Writes an inode entry back to disk.
 * Parameters:
 * - inode_number: which inode to write
 * - inode: pointer to the data to write
 * Returns 0 on success, -1 on invalid inode number.
 */
int inode_write(int inode_number, const INODE_ENTRY *inode) {
    // Validate inode number range
    if (inode_number < 0 || inode_number >= 1024) {
        fprintf(stderr, "Invalid inode number: %d\n", inode_number);
        return -1;
    }

    // Calculate block number that contains this inode
    int block_number = 3 + (inode_number / 16);

    INODE_BLOCK inode_block;
    block_read(block_number, &inode_block);

    // Overwrite the specific inode with new data
    inode_block.inodes[inode_number % 16] = *inode;

    // Write the updated inode block back to disk
    block_write(block_number, &inode_block);

    return 0;
}
