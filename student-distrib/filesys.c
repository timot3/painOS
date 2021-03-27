#include "filesys.h"

const static boot_blk_t* boot_blk;

int32_t file_open(const uint8_t filename){
    // read dentry by name
    return 0;
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

int32_t read_dentry_by_name();
int32_t read_dentry_by_index();

int32_t read_data(){

}
void filesys_init(void *fs){
    boot_blk = (boot_blk_t*)fs; // pointer?
    printf("n_dir_entries: %d | n_inodes: %d", boot_blk->n_dir_entries, boot_blk->n_inodes);

    // inode_ptr = &((inode_t*)fs)[1];
    // den_ptr = &((dentry_t*)fs)[1];
}