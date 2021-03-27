#ifndef FILESYS_H
#define FILESYS_H

#include "types.h"

#define RESERVED_DENTRY_SPACE 24
#define FOUR_BYTE 32
#define RESERVED_BOOT_BLK 52
#define NUM_DIR_ENTRIES 63
#define EIGHT_BYTE 64
#define NUM_D_BLKS 1023
#define ONE_KB 1024
#define FOUR_KB 4096

typedef struct dentry{
    char fname[FOUR_BYTE];
    uint32_t type; 
    uint32_t inode;
    uint8_t reserved[RESERVED_DENTRY_SPACE];
} dentry_t; // 64 B

typedef struct inode{
    int32_t len;
    int32_t dblocks[NUM_D_BLKS];
} inode_t; // 4 KB

typedef struct boot_blk{
    int32_t n_dir_entries;
    int32_t n_inodes;
    int32_t n_dblocks;
    char reserved[RESERVED_BOOT_BLK];
    dentry_t dir_entries[NUM_DIR_ENTRIES];
} boot_blk_t; // 4 KB

int32_t file_open(const uint8_t* filename);
int32_t file_read(int32_t fd, void* buf, int32_t nbytes);
int32_t write (int32_t fd, const void* buf, int32_t nbytes);
int32_t file_close();

int32_t dir_open();
int32_t dir_read();
int32_t dir_write();
int32_t dir_close();

int32_t read_dentry_by_name(const *inode_t inode);
int read_dentry_by_index();

int read_data();

void filesys_init(void *fs);
#endif /* filesys.h */
