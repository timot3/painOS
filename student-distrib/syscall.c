#include "syscall.h"
#include "filesys.h"

volatile uint8_t ret_status = -1;

char pid_arr[PROCESS_LIMIT] = {0, 0};

int32_t halt (uint8_t status) {
    ret_status = status;
    return status;

}
int32_t execute (const uint8_t* command) {
    int i, ret;
    uint8_t buf[CMD_MAX_LEN];
    // find file by name --> use func from filesys

    // parse until first space
    for (i = 0; i < CMD_MAX_LEN; i++) {
        if (command[i] == ' ' || command[i] == '\0') {
            if (i == 0) return -1;
            break;
        }
        buf[i] = command[i];
    }

    // make sure that the file exists, otherwise return -1
    if (file_open(buf) == -1) return -1;

    // check the first four characters for 0: 0x7f; 1: 0x45; 2: 0x4c; 3: 0x46
    // as per the MP documentation
    uint8_t first_32_bytes[CMD_MAX_LEN];
    file_read(0, first_32_bytes, CMD_MAX_LEN);
    if (first_32_bytes[0] != 0x7f || first_32_bytes[1] != 0x45 || first_32_bytes[2] != 0x4c || first_32_bytes[3] != 0x46)
        return -1;

    // TODO: later checkpoint
    // save arguments from command --> need for getargs

    // Get entry point

    // Set up paging

    // return 256 if dies by exception, value in range 0-255 if program executes a halt syscall,
    // then we return specified value in halt

    return ret_status;
}

/*
 * assign_pid
 *   DESCRIPTION: helper function, gives process first non-assigned value
 *   INPUTS: none
 *   RETURN VALUE: assigned process value or -1 if processes are full
 */
int assign_pid(void){
    int i;
    for(i=0; i<PROCESS_LIMIT; i++){
        if(pid_arr[i]==0){
            pid_arr[i]==1;
            return i;
        }
    }
    return -1;
}
void init_pcb (void){
    return -1
}

int allocate_pcb(int pid){
    return -1
}

int32_t read (int32_t fd, void* buf, int32_t nbytes) {
    return -1;

}
int32_t write (int32_t fd, const void* buf, int32_t nbytes) {
    return -1;

}
int32_t open (const uint8_t* filename) {

    uint32_t rval;
    
    return rval;


}
int32_t close (int32_t fd) {
    return -1;
}
int32_t getargs (uint8_t* buf, int32_t nbytes) {
    return -1;

}
int32_t vidmap (uint8_t** screen_start) {
    return -1;

}
int32_t set_handler (int32_t signum, void* handler_address) {
    return -1;

}
int32_t sigreturn (void) {
    return -1;

}
