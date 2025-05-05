//disk.h
#ifndef DISK_H
#define DISK_H

#include <stddef.h> // for size_t
#include <stdbool.h> // for bool type

#define DISK_NAME "/pdosfs"

extern void *disk_base;

// Disk management functions
int pdos_mkdisk(bool fresh);    // Create and map the disk
int pdos_mkfs(char *id);  // Format the disk
int block_read(int block_number, void *buffer);
int block_write(int block_number, const void *buffer);
int data_block_allocate(void);


#endif
