Pseudo File System by Athony Duan & Cameron Daly

Project Overview:
    The goal of this project is to implement an in-core (RAM-based) pseudo file system that mimics key aspects of a traditional inode-based file system, such as those used by UNIX and Linux. This file system uses a file-backed shared memory segment to simulate a disk and supports basic operations like creating files, writing and reading data, and managing a root directory.

Disk Layout:
    The pseudo-disk is a flat memory space divided into fixed-size blocks of 1024 bytes. It is structured as follows:

    Block #0 ----> Superblock (ID string)

    Block #1 ----> Inode Bitmap

    Block #2 ----> Data Bitmap

    Block #3-66 -----> Inode Blocks (64 blocks x 16 inodes = 1024 inodes)

    Block #67-1090 ----> Data Blocks (file content or directory entries)


Key Structures:

* Superblock: Contains an identifying string to recognize the file system.

* Inode Bitmap (Block 1): 512 shorts (8192 bits) track usage of the 1024 available inodes. A bit value of 1 indicates the inode is in use.

* Data Bitmap (Block 2): Tracks usage of the 1024 data blocks starting from block 67. Same format as inode bitmap.

* Inode Blocks (Blocks 3-66):
    * Each block holds 16 INODE_ENTRY structs.

    * Each INODE_ENTRY stores file metadata: size, modification time, used blocks, and data block pointers (up to 15).

* Data Blocks (Block 67+):
    * Used to store either file content or directory structures.

    * If pointed to by a directory inode, contains a DIR_BLOCK with an array of DIR_ENTRY structs.

    * If pointed to by a file inode, contains raw file data.


Data Structures:

* INODE_ENTRY:
    * Holds metadata about a file or directory: size, time, number of data blocks used, and which data blocks they are (64 bytes, 22 used for padding).

* DIR_ENTRY:
    * Contains filename, inode number, and entry type (file or directory) (32 bytes, 13 used for padding).

* DIR_BLOCK:
    * Contains multiple DIR_ENTRY structs (20 in our design due to a 51-byte custom structure).

* PDOS_FILE:
    * Runtime structure used to track open files: inode reference, current position, buffer of 1024 bytes, and modes (1035 bytes).

Supported Operations -- Specified Methods:

* int pdos_mkdisk(bool fresh) – Initializes a new shared memory region for the disk. Fresh tells us whether we're recreating the disk entirely. In disk.c.

* int pdos_mkfs(char* ID) – Formats the disk: writes superblock, bitmaps, root inode, and root directory, ID goes into block 0. In disk.c.

* PDOS_FILE *pdos_open(const char *fname, const char *mode) – Open or create a file. In fs.c.

* void pdos_fputc(PDOS_FILE *pf) – Write data to a file. In fs.c.
  
* int pdos_fgetc(int b, PDOS_FILE *pf) – Read data from a file. In fs.c.

* void pdos_fclose(PDOS_FILE *pf) – Finalize writing, flush buffers, and update inode. In fs.c.

* char** pdos_dir() – Return list of filenames in the root directory. In dir.c.
  
* int pdos_mkdir(const char *dirname) – Create a new subdirectory or the root directory and place "." and ".." within it. In fs.c.

Supported Operations -- Helper Methods:

* int block_read(int block_number, void *buffer) – Read operations at the block level, basically taking a block struct instance and populating its data with what we have in memory. Used as a helper method in a number of methods. In disk.c.
  
* int block_write(int block_number, const void *buffer) – Write operations at the block level, taking a block struct instance and populating its data into memory. Used as a helper method in a number of methods. In disk.c.

* int data_block_allocate() – Finds the next free data block. Used as a helper method in dir_add(), pdos_fputc(), and pdos_mkdir(). In disk.c.

* int dir_add(const char *filename, int inode_number, int type) – Add a directory entry of specified type and at specified inode_number if dir_lookup() returns -1, used as a helper method for pdos_open() and pdos_mkdir(). In dir.c.

* int dir_lookup(cost char* filename) – Search for directory entries, used as a helper method for pdos_open(). In dir.c.

* int inode_allocate() – Finds the next free inode. In inode.c. Used as a helper method in pdos_mkdir() and pdos_open().

* int inode_read(int inode_number, INODE_ENTRY *inode) – Read a specific inode. In inode.c. Used as a helper method in dir_lookup(), dir_add(), and pdos_dir().
  
* int inode_write(int inode_number, const INODE_ENTRY *inode) – Write to a specific inode. In inode.c. Used as a helper method in pdos_mkdir(), pdos_open(), and dir_add().
  
* int pdos_write(PDOS_FILE *pf, const char *buf, size_t len) – calls pdos_fputc() for as many times as specified and outputs from buffer. Helper method used in testing. In fs.c.

* bool pdos_exists(const char *fname) – returns -1 if the file name does not exist. Helper method used in testing. In fs.c.

* int pdos_read(PDOS_FILE *pf, char *buf, size_t len) – calls pdos_fgetc() for as many times as specified and inputs into buffer. Helper method used in testing. In fs.c.

* int pdos_write(PDOS_FILE *pf, const char *buf, size_t len) – calls pdos_fputc() for as many times as specified and outputs from buffer. Helper method used in testing. In fs.c.

# TESTING 

## Remove Shared Memory 
*   rm -f /dev/shm/pdosfs

## Look for Shared Memory 
*   ls -lh /dev/shm/pdosfs

## Inspect 
*   hexdump -C /dev/shm/pdosfs

## Test Static Library
*   make clean
*   make lib/libpdosfs.a
*   mv src src_hidden
*   make tests
*   ./test/for_submission