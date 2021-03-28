#include "filesys.h"

static boot_blk_t *boot_blk;
static uint32_t file_progress;
static inode_t *current_inode;
static dentry_t *current_dentry;

/*
 * file_open
 *   DESCRIPTION:
 *   INPUTS: *filename -
 *   OUTPUTS: none
 *   RETURN VALUE:
 */
int32_t file_open(const uint8_t *filename) {
    // read dentry by name
    dentry_t dentry; 
    file_progress = 0;
    return read_dentry_by_name(fname, &dentry);
    
}

/*
 * file_read
 *   DESCRIPTION:
 *   INPUTS: fd -
 *            *buf -
 *            nbytes -
 *   OUTPUTS: none
 *   RETURN VALUE:
 */
int32_t file_read(int32_t fd, void *buf, int32_t nbytes) {
    return read_data((uint32_t)current_inode, file_progress, (uint8_t *)buf, nbytes); // 0 when done, otherwise nbytes read
}

/*
 * file_write
 *   DESCRIPTION: Does nothing for now
 *   INPUTS: fd - never used, docs said we needed it
 *            *buf - never used, docs said we needed it
 *            nbytes - never used, docs said we needed it
 *   OUTPUTS: none
 *   RETURN VALUE: Always returns -1
 */
int32_t file_write(int32_t fd, const void *buf, int32_t nbytes) {
    printf("Read-Only for now");
    return -1;
}

/*
 * file_close
 *   DESCRIPTION:
 *   INPUTS: fd - never used, docs said we needed it
 *   OUTPUTS: none
 *   RETURN VALUE: Always returns 0
 */
int32_t file_close(int32_t fd) {
    current_inode  = NULL;
    current_dentry = NULL;
    file_progress  = 0;

    // undo open
    return 0;
}

/*
 * dir_open
 *   DESCRIPTION:
 *   INPUTS: *filename - the name of the file to be opened
 *   OUTPUTS: none
 *   RETURN VALUE: Return -1 if file not found, 0 otherwise
 */
int32_t dir_open(const uint8_t *filename) {
    // name == "."
    return read_dentry_by_name(filename, 0);
}

/*
 * dir_read
 *   DESCRIPTION:
 *   INPUTS: fd - never used, docs said we needed it
 *            *buf - never used, docs said we needed it
 *            nbytes - never used, docs said we needed it
 *   OUTPUTS: none
 *   RETURN VALUE: Always returns 0
 */
int32_t dir_read(int32_t fd, void *buf, int32_t nbytes) {
    // read dentry by index
    // reads one at a time
    //return read_dentry_by_index(idx, inputDentry); // 0 when done, otherwise nbytes

    int i, j;
    int ret = 0;
    dentry_t inputDentry;

    for(i = 0; ret != -1; i++) {
        ret = read_dentry_by_index(i, &inputDentry);
        if(ret == -1)
            break;

        printf("file_name: ");
        for(j = 0; j < MAX_CHAR; j++)
            putc(inputDentry.fname[j]);

        printf(", file_type: %d", inputDentry.type);
        printf(", file_size: %d\n", ((inode_t*)(inputDentry.inode))->len);
    }

    return 0;
}

/*
 * dir_write
 *   DESCRIPTION:
 *   INPUTS: fd - never used, docs said we needed it
 *            *buf - never used, docs said we needed it
 *            nbytes - never used, docs said we needed it
 *   OUTPUTS: none
 *   RETURN VALUE: Always returns -1
 */
int32_t dir_write(int32_t fd, const void *buf, int32_t nbytes) {
    printf("You can't write a directory, silly!");
    return -1;
}

/*
 * dir_close
 *   DESCRIPTION:
 *   INPUTS: fd - never used, docs said we needed it
 *   OUTPUTS: none
 *   RETURN VALUE: Always returns 0
 */
int32_t dir_close(int32_t fd) {
    current_inode  = NULL;
    current_dentry = NULL;
    file_progress  = 0;
    return 0;
}

/*
 * read_dentry_by_name
 *   DESCRIPTION:
 *   INPUTS: *fname -
 *           *dentry -
 *   OUTPUTS: none
 *   RETURN VALUE: Return -1 if file not found, 0 otherwise
 */
int32_t read_dentry_by_name(const uint8_t *fname, dentry_t *dentry) {
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
        dentry = &(boot_blk->dir_entries[i]);
        current_inode  = &((inode_t *)boot_blk)[current_dentry->inode + 1];
        file_progress  = 0;
        return 0;
    }

    // return 1 if not found
    return -1;
}

/*
 * read_dentry_by_index
 *   DESCRIPTION:
 *   INPUTS: idx - index of dentry to view
 *            *inputDentry - dentry to copy info to
 *   OUTPUTS: none
 *   RETURN VALUE: Return -1 if desired value is out of range, 0 otherwise
 */
int32_t read_dentry_by_index(uint32_t idx, dentry_t *inputDentry) {
    int i;

    if(idx < 0 || idx == boot_blk->n_dir_entries)   // no more dirs to read
        return -1;

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

/*
 * read_data
 *   DESCRIPTION:
 *   INPUTS: inode -
 *            offset -
 *            *buf -
 *            nbytes -
 *   OUTPUTS: none
 *   RETURN VALUE: Always returns 0
 */
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t *buf, uint32_t nbytes) {
    // uses file_progress, and current_inode
    uint32_t *dblock_addr;
    int i, dblocks_idx, dblocks_offset, fsize, iterations;

    fsize = current_inode->len;

    if(file_progress >= fsize) {
        // there is no more to read
        return -1;
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

/*
 * filesys_init
 *   DESCRIPTION: Initializes filesystem variables
 *   INPUTS: *fs - pointer to start of boot block
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
void filesys_init(void *fs) {
    boot_blk = (boot_blk_t *)fs; // pointer?
    printf("n_dir_entries: %d | n_inodes: %d",
           boot_blk->n_dir_entries,
           boot_blk->n_inodes);

    // inode_ptr = &((inode_t*)fs)[1];
    // den_ptr = &((dentry_t*)fs)[1];
}
