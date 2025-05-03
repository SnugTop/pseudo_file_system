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
    * Holds metadata about a file or directory: size, time, data block usage.

* DIR_ENTRY:
    * Contains filename, inode number, and entry type (file or directory).

* DIR_BLOCK:
    * Contains multiple DIR_ENTRY structs (20 in our design due to a 51-byte custom structure).

* PDOS_FILE:
    * Runtime structure used to track open files: mode, current position, buffer, inode reference.


Supported Operations:

* pdos_mkdisk() – Initializes a new shared memory region for the disk.

* pdos_mkfs() – Formats the disk: writes superblock, bitmaps, root inode, and root directory.

* block_read()/block_write() – Read/write operations at the block level.

* inode_allocate() – Finds the next free inode.

* data_block_allocate() – Finds the next free data block.

* inode_read()/inode_write() – Read/write a specific inode.

* dir_add()/dir_lookup() – Add or search for directory entries.

* pdos_dir() – Return list of filenames in the root directory.

* pdos_open() – Open or create a file.

* pdos_fputc()/pdos_fgetc() – Write/read data to/from a file.

* pdos_fclose() – Finalize writing, flush buffers, and update inode.