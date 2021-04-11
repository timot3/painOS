#include "filesys.h"

static boot_blk_t *boot_blk;
static uint32_t file_progress;
static dentry_t current_dentry;
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
    file_progress = 0;
    ret =  read_dentry_by_name(filename, &dentry);

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
 *   INPUTS: fd - file descriptor (not currently used)
 *           *buf - buffer to store returned data
 *           nbytes - size of given buffer
 *   OUTPUTS: none
 *   RETURN VALUE: Amount of bytes read
 */
int32_t file_read(int32_t fd, uint8_t *buf, int32_t nbytes) {
    return read_data(current_dentry.inode, 0, buf, nbytes); // 0 when done, otherwise nbytes read
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
    file_progress  = 0;
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
    return read_dentry_by_name(filename, 0);
}

/*
 * dir_read
 *   DESCRIPTION: Gets names, file types, and size of all files in current directory
 *   INPUTS: fd - never used, docs said we needed it
 *                May be useful for future checkpoints
 *           *buf - buffer to copy info to
 *            nbytes - size of buffer
 *   OUTPUTS: Prints contents of directory
 *   RETURN VALUE: Always returns 0
 */
int32_t dir_read(int32_t fd, void *buf, int32_t nbytes) {
    int i = fd, j;
    int ret = 0;
    dentry_t inputDentry;
    uint8_t fN[11] = "file_name: ", fT[13] = ", file_type: ", fS[13] = ", file_size: ";
    int loc = 0;

    // Because TA said no to for loop here -> said to look at ls function and
    // base this function off of that
    // Read items in directory
    ret = read_dentry_by_index(i, &inputDentry);
    if(ret == -1)
        return -1;

    // Copy "file_name: " string to buffer
    for(j = 0; j < 11; j++) {
        // Return if buffer is now full
        if(loc > nbytes) {
            // Add NULL character at very end of buffer
            ((uint8_t*)buf)[loc - 1] = '\0';
            return 0;
        }

        ((uint8_t*)buf)[loc] = fN[j];
        loc++;
    }

    // Copy filename to buffer
    for(j = 0; j < MAX_FILENAME_LEN; j++) {
        // Return if buffer is now full
        if(loc > nbytes) {
            // Add NULL character at very end of buffer
            ((uint8_t*)buf)[loc - 1] = '\0';
            return 0;
        }

        // Don't send NULl characters, change them to spaces
        if(inputDentry.fname[j] == 0)
            ((uint8_t*)buf)[loc] = ' ';
        else
            ((uint8_t*)buf)[loc] = inputDentry.fname[j];
        loc++;
    }

    // Copy ", file_type: " string to buffer
    for(j = 0; j < 13; j++) {
        // Return if buffer is now full
        if(loc > nbytes) {
            // Add NULL character at very end of buffer
            ((uint8_t*)buf)[loc - 1] = '\0';
            return 0;
        }

        ((uint8_t*)buf)[loc] = fT[j];
        loc++;
    }

    // Convert dentry type int to string
    uint8_t intBuff[10];
    itoa(inputDentry.type, (int8_t*)intBuff, 10);

    // Copy type to buffer
    for(j = 0; j < 10; j++) {
        // Return if buffer is now full
        if(loc > nbytes) {
            // Add NULL character at very end of buffer
            ((uint8_t*)buf)[loc - 1] = '\0';
            return 0;
        }

        // Stop copying if NULL character reached
        if(intBuff[j] == 0)
            break;
        ((uint8_t*)buf)[loc] = intBuff[j];
        loc++;
    }

    // Copy ", file_size: " string to buffer
    for(j = 0; j < 13; j++) {
        // Return if buffer is now full
        if(loc > nbytes) {
            // Add NULL character at very end of buffer
            ((uint8_t*)buf)[loc - 1] = '\0';
            return 0;
        }

        ((uint8_t*)buf)[loc] = fS[j];
        loc++;
    }

    // Convert dentry size int to string
    uint8_t intBuff2[10];
    itoa(((inode_t*)(inputDentry.inode + boot_blk + 1))->len, (int8_t*)intBuff2, 10);

    // Copy size to buffer
    for(j = 0; j < 10; j++) {
        // Return if buffer is now full
        if(loc > nbytes) {
            // Add NULL character at very end of buffer
            ((uint8_t*)buf)[loc - 1] = '\0';
            return 0;
        }

        // Stop copying if NULL character reached
        if(intBuff2[j] == 0)
            break;
        ((uint8_t*)buf)[loc] = intBuff2[j];
        loc++;
    }

    // Add newline at end of each entry
    ((uint8_t*)buf)[loc] = '\n';
    loc++;

    if(loc > nbytes) {
        // Add NULL character at very end of buffer
        ((uint8_t*)buf)[loc - 1] = '\0';
        return 0;
    }

    // Add NULL character at very end of buffer
    ((uint8_t*)buf)[loc] = '\0';

    return 0;
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
    // reset the current file and file data entry.
    file_progress  = 0;
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
    int i, j, cont = 0;

    // for each dir entry, search to see if the filename matches
    for(i = 0; i < boot_blk->n_dir_entries; i++) {
        // copy the name
        cont = 0;
        // check each character
        for(j = 0; j < MAX_FILENAME_LEN; j++) {
            if(fname[j] != (boot_blk->dir_entries)[i].fname[j]) {
                cont = 1;
                break;
            }
        }

        // if there was a difference, continue to next dir entry
        if(cont) continue;

        // otherwise set the global vars and return 0
        dentry_t currDentry = boot_blk->dir_entries[i];
        for(j = 0; j < MAX_FILENAME_LEN; j++) {
            dentry->fname[j] = currDentry.fname[j];
        }

        dentry->type = currDentry.type;
        dentry->inode = currDentry.inode;

        // current_inode  = (inode_t *)(boot_blk + ((dentry->inode + 1)*FOUR_KB));
        file_progress  = 0;
        return 0;
    }

    // return 1 if not found
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
    printf("n_dir_entries: %d | n_inodes: %d",
           boot_blk->n_dir_entries,
           boot_blk->n_inodes);
}
