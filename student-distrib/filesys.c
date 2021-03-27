#include "filesys.h"

const static boot_blk_t* boot_blk;
static uint32_t file_progress;
static inode_t* current_inode;
static dentry_t* current_dentry;

int32_t file_open(const uint8_t* filename){
    // read dentry by name
    file_progress = 0;
    return read_dentry_by_name(filename);
}
int32_t file_read(int32_t fd, void* buf, int32_t nbytes){
    // 
    return 0; // 0 when done, otherwise nbytes read 
}
int32_t file_write(){
    printf("Read-Only for now")
    return -1;
}
int32_t file_close(){
    // undo open
    return 0; 
}

int32_t dir_open(){
    // read dentry by name
    return 0;
}
int32_t dir_read(){
    // read dentry by index 
    // reads one at a time
    return 0; // 0 when done, otherwise nbytes 
}
int32_t dir_write(){
    printf("You can't write a directory, silly!")
    return -1; 
}
int32_t dir_close(){
    return 0;
}

int32_t read_dentry_by_name(const uint8_t* fname){
    int fname_length = strlen(fname);
    int i, j;
    bool cont = false; 
    dentry_t dir_entries[NUM_DIR_ENTRIES] = boot_blk->dir_entries;
    for (i=0; i<boot_blk->n_dir_entries; i++){
        char dir_name[MAX_CHAR] = dir_entries[i].fname;
        for (j=0; j<fname_length; j++){
            if (fname[j] != dir_name[j]){
                cont = true;
                break
            }
        }
        if (cont)
            continue;
        current_dentry = boot_blk->dir_entries[i];
        current_inode = current_dentry.inode;
        file_progress = 0;
        return 0;
    }
    return -1;
}
int32_t read_dentry_by_index();

int32_t read_data(){

}
void filesys_init(void *fs){
    boot_blk = (boot_blk_t*)fs; // pointer?
    printf("n_dir_entries: %d | n_inodes: %d", boot_blk->n_dir_entries, boot_blk->n_inodes);

    // inode_ptr = &((inode_t*)fs)[1];
    // den_ptr = &((dentry_t*)fs)[1];
}