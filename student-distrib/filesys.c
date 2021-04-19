#include "filesys.h"
#include "syscall.h"

static boot_blk_t *boot_blk;

/*
 * file_open
 *   DESCRIPTION: Opens file, stores found dentry in global current_dentry variable
 *   INPUTS: *filename - the name of the file to read
 *   OUTPUTS: none
 *   RETURN VALUE: Return -1 if file not found, 0 otherwise
 */
int32_t file_open(const uint8_t *filename) {
    // read dentry by name
    int32_t ret, j;
    dentry_t dentry;
    ret = read_dentry_by_name(filename, &dentry);

	if(ret == -1)
		return -1;

    // Copy name to global dentry
    for(j = 0; j < MAX_FILENAME_LEN; j++) {
        current_dentry.fname[j] = dentry.fname[j];
    }

    // Copy other attributes to global dentry
    current_dentry.type = dentry.type;
    current_dentry.inode = dentry.inode;

    return ret;
}

/*
 * file_read
 *   DESCRIPTION: Reads data of the currently open file
 *   INPUTS: fd - file descriptor
 *           *buf - buffer to store returned data
 *           nbytes - size of given buffer
 *   OUTPUTS: none
 *   RETURN VALUE: Amount of bytes read
 */
int32_t file_read(int32_t fd, void *buf, int32_t nbytes) {
    // Get latest PCB and read data from it
	pcb_t *currPCB = get_latest_pcb();

	int ret = read_data(currPCB->fd_items[fd].inode, currPCB->fd_items[fd].file_position, buf, nbytes);

    // Update file position
	currPCB->fd_items[fd].file_position += ret;

	// Ensure read data didn't fail
	if(ret == -1)
		return 0;

	return ret;
}

/*
 * file_write
 *   DESCRIPTION: Does nothing for now
 *   INPUTS: fd - never used, docs said we needed it
 *              May be useful for future checkpoints
 *           *buf - never used, docs said we needed it
 *              May be useful for future checkpoints
 *           nbytes - never used, docs said we needed it
 *              May be useful for future checkpoints
 *   OUTPUTS: none
 *   RETURN VALUE: Always returns -1
 */
int32_t file_write(int32_t fd, const void *buf, int32_t nbytes) {
    printf("Read-Only for now");
    return -1;
}

/*
 * file_close
 *   DESCRIPTION: Closes currently open file
 *   INPUTS: fd - never used, docs said we needed it.
 *      May be useful for future checkpoints
 *   OUTPUTS: none
 *   RETURN VALUE: Always returns 0
 */
int32_t file_close(int32_t fd) {
    // reset current data and data entry
    // undo open
    return 0;
}

/*
 * dir_open
 *   DESCRIPTION: Opens given directory
 *   INPUTS: *filename - the name of the file (directory) to be opened
 *   OUTPUTS: none
 *   RETURN VALUE: Return -1 if file not found, 0 otherwise
 */
int32_t dir_open(const uint8_t *filename) {
    // name == "."
	dentry_t dentry;
    return read_dentry_by_name(filename, &dentry); //fails for some reason
}

/*
 * dir_read
 *   DESCRIPTION: Gets names, file types, and size of all files in current directory
 *   INPUTS: fd - never used, docs said we needed it
 *                May be useful for future checkpoints
 *           *buf - buffer to copy info to
 *            nbytes - size of buffer
 *   OUTPUTS: Prints contents of directory
 *   RETURN VALUE: Number of bytes copied to buffer, 0 if unable to find dentry
 */
int32_t dir_read(int32_t fd, void *buf, int32_t nbytes) {
    // Get correct PCB
    pcb_t *currPCB = get_latest_pcb();
    dentry_t inputDentry;

    // Read dentry
    int ret = read_dentry_by_index(currPCB->fd_items[fd].file_position, &inputDentry), i;

    // Return if file doesn't exist
    if(ret == -1)
        return 0;

    // Increment file position
    currPCB->fd_items[fd].file_position++;

    // Copy filename to input buffer
    for(i = 0; i < MAX_FILENAME_LEN; i++) {
        // Return once end of string found
		((uint8_t*)buf)[i] = inputDentry.fname[i];
        if(inputDentry.fname[i] == '\0')
            return i;
    }

    return i;
}

/*
 * dir_write
 *   DESCRIPTION: Writes a directory.
 *   INPUTS: fd - never used, docs said we needed it
 *              May be useful for future checkpoints
 *           *buf - never used, docs said we needed it
 *              May be useful for future checkpoints
 *           nbytes - never used, docs said we needed it
 *              May be useful for future checkpoints
 *   OUTPUTS: none
 *   RETURN VALUE: Always returns -1
 */
int32_t dir_write(int32_t fd, const void *buf, int32_t nbytes) {
    printf("This feature is not yet implemented.");
    return -1;
}

/*
 * dir_close
 *   DESCRIPTION: Stops reading the current directory.
 *   INPUTS: fd - never used, docs said we needed it
 *   OUTPUTS: none
 *   RETURN VALUE: Always returns 0
 */
int32_t dir_close(int32_t fd) {
    return 0;
}

/*
 * read_dentry_by_name
 *   DESCRIPTION:
 *   INPUTS: *fname  - Name of the data entry that we want to read.
 *           *dentry - Pointer to dentry that we want to populate
 *   OUTPUTS: none
 *   RETURN VALUE: Return -1 if file not found, 0 otherwise
 */
int32_t read_dentry_by_name(const uint8_t *fname, dentry_t *dentry) {
	// inits
	uint32_t i, j, cont = 0;

    // for each dir entry, search to see if the filename matches
    for(i = 0; i < boot_blk->n_dir_entries; i++) {
        // copy the name
        cont = 0;
        // check each character
        for(j = 0; j < MAX_FILENAME_LEN; j++) {
			// Break and null character
			if(fname[j] == '\0' && (boot_blk->dir_entries)[i].fname[j] == '\0')
				break;

            if(fname[j] != (boot_blk->dir_entries)[i].fname[j]) {
                cont = 1;
                break;
            }
        }

        // if there was a difference, continue to next dir entry
        if(cont) continue;

        // otherwise set the global vars and return 0
        for(j = 0; j < MAX_FILENAME_LEN; j++)
            dentry->fname[j] = boot_blk->dir_entries[i].fname[j];

        dentry->type = boot_blk->dir_entries[i].type;
        dentry->inode = boot_blk->dir_entries[i].inode;

        // current_inode  = (inode_t *)(boot_blk + ((dentry->inode + 1)*FOUR_KB));
        return 0;
    }

    // return -1 if not found
    return -1;
}

/*
 * read_dentry_by_index
 *   DESCRIPTION: Gets dentry information at a given index
 *   INPUTS: idx - index of dentry to view
 *            *inputDentry - dentry to copy info to
 *   OUTPUTS: none
 *   RETURN VALUE: Return -1 if desired value is out of range, 0 otherwise
 */
int32_t read_dentry_by_index(uint32_t idx, dentry_t *input_dentry) {
    int i;

    if(idx < 0 || idx >= boot_blk->n_dir_entries)   // no more dirs to read
        return -1;

    current_dentry = (dentry_t)boot_blk->dir_entries[idx];

    // Deep copy of all variables in dentry_t struct
    for(i = 0; i < MAX_FILENAME_LEN; i++)
        input_dentry->fname[i] = current_dentry.fname[i];

    input_dentry->type = current_dentry.type;

    // boot_blk + 1 for offset of inode starting position
    input_dentry->inode = current_dentry.inode;

    return 0;
}

/*
 * read_data
 *   DESCRIPTION: Reads data of given inode index
 *   INPUTS: inode - index of inode
 *           offset - offset of data read
 *           *buf - buffer to copy info into
 *           nbytes - size of buffer
 *   OUTPUTS: none
 *   RETURN VALUE: Always returns 0
 */
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t *buf, uint32_t nbytes) {
    // Bounds checking
    if(inode > boot_blk->n_inodes || inode < 0 || offset < 0 || buf == NULL)
        return -1;

    // Define variables
    int i, read = 0;
    inode_t *start = (inode_t *)(boot_blk + 1 + inode);
    dblock_t *currData, *startData = (dblock_t *)(boot_blk + 1 + boot_blk->n_inodes);
    int whenToStop = start->len - offset;

    // Loop through number of bytes asked for (or until all data read)
    for(i = 0; i < nbytes && read <= whenToStop; i++) {
        // Set current buffer location to data read
        currData = (dblock_t *)(startData + start->dblocks[offset / FOUR_KB]);
        buf[i] = currData->data[offset % FOUR_KB];

        // Increment counters
        offset++;
        read++;
    }

    return read;
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
    printf("n_dir_entries: %d | n_inodes: %d\n",
           boot_blk->n_dir_entries,
           boot_blk->n_inodes);
}
