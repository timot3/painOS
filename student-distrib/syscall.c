#include "syscall.h"
#include "filesys.h"

volatile uint8_t ret_status = -1;

char pid_arr[PROCESS_LIMIT] = {0, 0};

int32_t halt (uint8_t status) {
    ret_status = status;
    return status;

}
/* int32_t execute (const uint8_t* command) {
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
} */

int32_t execute (const uint8_t* command) {
    int pid = assign_pid();
    if (pid == -1) return -1;
    pcb_t* pcb = allocate_pcb(pid);
    int parse_error = parse_command(command, pcb);

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

/*
 * unassign_pid
 *   DESCRIPTION: helper function, unassigns the pid specified
 *   INPUTS: pid you want unassigned
 *   RETURN VALUE: 1 if success, -1 if pid is already unassigned
 */
int unassign_pid(int pid){
    if (pid_arr[pid] == 1){
        pid_arr[pid] == 0;
        return 1;
    } 
    else
        return -1;
}

/*
 * allocate_pcb
 *   DESCRIPTION: get pointer to pcb location and initialize stdin and stdout
 *   INPUTS: pid of pcb that you want
 *   RETURN VALUE: pcb location
 */
pcb_t* allocate_pcb(int pid){
    //pcb is located at top of kernel stack (which is at bottom of kernel page) for process
    pcb_t* pcb = (pcb_t*)(KERNEL_PAGE_BOT - (pid + 1) * KERNEL_STACK_SIZE);

    //stdin in position 0
    pcb -> fd_items[0].file_op_jmp = stdin_table;
    pcb -> fd_items[0].inode = 0;
    pcb -> fd_items[0].file_position = 0;
    pcb -> fd_items[0].flags = 1;

    //stdout in position 1
    pcb -> fd_items[1].file_op_jmp = stdout_table;
    pcb -> fd_items[1].inode = 0;
    pcb -> fd_items[1].file_position = 0;
    pcb -> fd_items[1].flags = 1;

    return pcb;
}

/*
 * parse_comand
 *   DESCRIPTION: helper function parse execute function ,move arguments to pcb for safekeeping
 *   INPUTS: command and pcb_pointer
 *   RETURN VALUE: if successful 1, if fail -1
 */
int parse_comand(const uint8_t* command, pcb_t* pcb){
    uint8_t exec_file[CMD_MAX_LEN];
    for (i = 0; i < CMD_MAX_LEN; i++) {
        if (command[i] == ' ' || command[i] == '\0' || command[i] == '\n') {
            if (i == 0) return -1;
            else{
                
            }
            break;
        }
        exec_file[i] = command[i];
    }
}

int32_t read (int32_t fd, void* buf, int32_t nbytes) {
    return -1;

}
int32_t write (int32_t fd, const void* buf, int32_t nbytes) {
    return -1;

}
int32_t open (const uint8_t* filename) {
    return -1;
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
