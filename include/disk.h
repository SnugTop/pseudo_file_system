//disk.h
#ifndef DISK_H
#define DISK_H

#include <stddef.h> // for size_t

#define DISK_NAME "/pdosfs"

extern void *disk_base;

// Disk management functions
int pdos_mkdisk(void);    // Create and map the disk
int pdos_mkfs(char *id);  // Format the disk
int block_read(int block_number, void *buffer);
int block_write(int block_number, const void *buffer);
int data_block_allocate(void);


#endif
