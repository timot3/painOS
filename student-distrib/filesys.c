#include "filesys.h"

static boot_blk_t *boot_blk;
static uint32_t file_progress;
static inode_t *current_inode;
static dentry_t *current_dentry;

int32_t file_open(const uint8_t *filename) {
    // read dentry by name
    file_progress = 0;
    return read_dentry_by_name(filename);
}

int32_t file_read(int32_t fd, void *buf, int32_t nbytes) {
    return read_data(fd, (uint32_t *)buf, nbytes); // 0 when done, otherwise
                                                   // nbytes read
}

int32_t file_write() {
    printf("Read-Only for now");
    return -1;
}

int32_t file_close() {
    current_inode  = NULL;
    current_dentry = NULL;
    file_progress  = 0;

    // undo open
    return 0;
}

int32_t dir_open(const uint8_t *filename) {
    // name == "."
    return read_dentry_by_name(filename);
}

int32_t dir_read(uint16_t idx, dentry_t *inputDentry) {
    // read dentry by index
    // reads one at a time
    return read_dentry_by_index(idx, inputDentry); // 0 when done, otherwise nbytes
}

int32_t dir_write() {
    printf("You can't write a directory, silly!");
    return -1;
}

int32_t dir_close() {
    current_inode  = NULL;
    current_dentry = NULL;
    file_progress  = 0;
    return 0;
}

int32_t read_dentry_by_name(const uint8_t *fname) {
    // inits
    int i, j, fname_length, cont;

    cont = 0;

    // get strlen
    fname_length = strlen((int8_t *)fname);

    // for each dir entry
    for(i = 0; i < boot_blk->n_dir_entries; i++) {
        // copy the name

        // check each character
        for(j = 0; j < fname_length; j++) {
            if(fname[j] != (boot_blk->dir_entries)[i].fname[j]) {
                cont = 1;
                break;
            }
        }

        // if there was a difference, continue to next dir entry
        if(cont) continue;

        // otherwise set the global vars and return 0
        current_dentry = &(boot_blk->dir_entries[i]);
        current_inode  = &((inode_t *)boot_blk)[current_dentry->inode + 1];
        file_progress  = 0;
        return 0;
    }

    // return 1 if not found
    return -1;
}

int32_t read_dentry_by_index(uint16_t idx, dentry_t *inputDentry) {
    int i;
    
    if(idx == boot_blk->n_dir_entries) return -1;  // no more dirs to read

    current_dentry = &(boot_blk->dir_entries[idx]);

    // Deep copy of all variables in dentry_t struct
    for(i = 0; i < MAX_CHAR; i++)
        inputDentry->fname[i] = current_dentry->fname[i];

    inputDentry->type = current_dentry->type;

    // boot_blk + 1 for offset of inode starting position
    inputDentry->inode = current_dentry->inode + 1 + boot_blk;

    for(i = 0; i < RESERVED_DENTRY_SPACE; i++)
        inputDentry->reserved[i] = current_dentry->reserved[i];

    return 0;
}

int32_t read_data(int32_t fd, uint32_t *buf, int32_t nbytes) {
    // uses file_progress, and current_inode
    uint32_t *dblock_addr;
    int i, dblocks_idx, dblocks_offset, fsize, iterations;

    fsize = current_inode->len;

    if(file_progress >= fsize) {
        return -1;

        // there is no more to read
    }

    // keep reading
    // start at where we left off
    // go until we read nbytes (or return early)
    for(i = file_progress; i < file_progress + nbytes; i++) {
        // represents number of bytes read so far
        iterations = i - file_progress;

        if(i == fsize) // if we finish reading
            return iterations;

        // get the dblock index (in inode)
        dblocks_idx = i / FOUR_KB;

        // get the address of the dblock
        // bootblk + inodes + idx of dblock (in storage)
        dblock_addr = (uint32_t *)boot_blk +
                      (1 + boot_blk->n_inodes +
                       current_inode->dblocks[dblocks_idx]) * FOUR_KB;

        // get offset in the dblock
        dblocks_offset = i % FOUR_KB;

        // set ptr to represent current position
        dblock_addr += dblocks_offset;

        // copy data into buf
        buf[iterations] = dblock_addr[dblocks_offset];
    }
    file_progress = i;
    return iterations;
}

void filesys_init(void *fs) {
    boot_blk = (boot_blk_t *)fs; // pointer?
    printf("n_dir_entries: %d | n_inodes: %d",
           boot_blk->n_dir_entries,
           boot_blk->n_inodes);

    // inode_ptr = &((inode_t*)fs)[1];
    // den_ptr = &((dentry_t*)fs)[1];
}
