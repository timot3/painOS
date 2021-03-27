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

typedef struct dentry{
    char fname[MAX_CHAR];
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

int32_t read_dentry_by_name(const uint8_t* fname);
int32_t read_dentry_by_name(const uint8_t* fname);

int32_t read_data(int32_t fd, void* buf, int32_t nbytes);

void filesys_init(void *fs);
#endif /* filesys.h */
