#ifndef FILESYS_H
#define FILESYS_H

#include "types.h"
#include "lib.h" // putc


#define RESERVED_DENTRY_SPACE 24
#define FOUR_BYTE 32
#define MAX_CHAR 32
#define RESERVED_BOOT_BLK 52
#define MAX_DIR_ENTRIES 63
#define EIGHT_BYTE 64
#define NUM_D_BLKS 1023
#define ONE_KB 1024
#define FOUR_KB 4096

// data entry
typedef struct dentry{
    char fname[MAX_CHAR];
    uint32_t type;
    uint32_t inode; // inode index
    uint8_t reserved[RESERVED_DENTRY_SPACE];
} dentry_t; // 64 B
/*
 * a structure on the disk that represents a file, directory,
 * symbolic link, etc. Inodes do not contain the data of the
 * file / directory / etc. that they represent.
 */
typedef struct inode{
    int32_t len;
    int32_t dblocks[NUM_D_BLKS];
} inode_t; // 4 KB

// Starting location of the filesystem
typedef struct boot_blk{
    int32_t n_dir_entries;
    int32_t n_inodes;
    int32_t n_dblocks;
    char reserved[RESERVED_BOOT_BLK];
    dentry_t dir_entries[MAX_DIR_ENTRIES];
} boot_blk_t; // 4 KB

// Functions for working with files
int32_t file_open(const uint8_t* filename);
int32_t file_read(int32_t fd, uint8_t* buf, int32_t nbytes);
int32_t file_write(int32_t fd, const void *buf, int32_t nbytes);
int32_t file_close(int32_t fd);

// Functions for working with directories
int32_t dir_open(const uint8_t* filename);
int32_t dir_read(int32_t fd, void *buf, int32_t nbytes);
int32_t dir_write(int32_t fd, const void *buf, int32_t nbytes);
int32_t dir_close(int32_t fd);

// Functions for reading data entries
int32_t read_dentry_by_name(const uint8_t *fname, dentry_t *dentry);
int32_t read_dentry_by_index(uint32_t idx, dentry_t *inputDentry);

// Function to read data
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t *buf, uint32_t nbytes);

// Init the filesystem
void filesys_init(void *fs);
#endif /* filesys.h */
