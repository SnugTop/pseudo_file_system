#ifndef DISK_H
#define DISK_H

#include <stddef.h> // for size_t

#define DISK_NAME "/pdosfs"  // you can change this if needed
#define DISK_SIZE (1091 * 1024)

extern void *disk_base;  // Global pointer to mapped disk memory

int pdos_mkdisk(void);    // Create and map the disk
int block_read(int block_number, void *buffer);
int block_write(int block_number, const void *buffer);

#endif 
