#include "syscall.h"
#include "filesys.h"

volatile uint8_t ret_status = -1;

file_op_table_t rtc_table = {
    .open = rtc_open,
    .read = rtc_read,
    .write = rtc_write,
    .close = rtc_close
};

file_op_table_t file_table = {
    .open = file_open,
    .read = file_read,
    .write = file_write,
    .close = file_close
};

file_op_table_t dir_table = {
    .open = dir_open,
    .read = dir_read,
    .write = dir_write,
    .close = dir_close
};

file_op_table_t stdin_table = {
    .open = std_bad_call,
    .read = terminal_read,
    .write = std_bad_call,
    .close = std_bad_call
};

file_op_table_t stdout_table = {
    .open = std_bad_call,
    .read = std_bad_call,
    .write = terminal_write,
    .close = std_bad_call
};

char pid_arr[PROCESS_LIMIT] = {0, 0};

int32_t halt (uint8_t status) {
    ret_status = status;
    return status;

}

int32_t execute (const uint8_t* command) {
    int pid = assign_pid();
    if (pid == -1) return -1;
    pcb_t* pcb = allocate_pcb(pid);
    int parse = parse_command(command, pcb, pid);
    if (parse == -1){
        unassign_pid(pid);
        return -1;
    }
return -1;
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
            pid_arr[i] = 1;
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
        pid_arr[pid] = 0;
        return 1;
    }
    else
        return -1;
}

/*
 * get_latest_pid
 *   DESCRIPTION: helper function, gets the most recently assigned pid
 *   INPUTS: none
 *   RETURN VALUE: idx of most recently used pid or -1 if none
 */
int get_latest_pid(){
    int i;
    for (i = 0; i < PROCESS_LIMIT; i++) {
        if (pid_arr[i] == 1)
            return i;
    }
    return -1;
}

/*
 * get_pcb_addr
 *   DESCRIPTION: get pointer to pcb location of given pid
 *   INPUTS: pid of pcb that you want
 *   RETURN VALUE: pcb location
 */
pcb_t* get_pcb_addr(int pid) {
    return (pcb_t*)(KERNEL_PAGE_BOT - (pid + 1) * KERNEL_STACK_SIZE);
}
/*
 * allocate_pcb
 *   DESCRIPTION: get pointer to pcb location and initialize stdin and stdout
 *   INPUTS: pid of pcb that you want
 *   RETURN VALUE: pcb location
 */
pcb_t* allocate_pcb(int pid){
    //pcb is located at top of kernel stack (which is at bottom of kernel page) for process
    pcb_t* pcb = get_pcb_addr(pid);

    //stdin in position 0
    pcb -> fd_items[STDIN_IDX].file_op_jmp = stdin_table;
    pcb -> fd_items[STDIN_IDX].inode = 0;
    pcb -> fd_items[STDIN_IDX].file_position = 0;
    pcb -> fd_items[STDIN_IDX].flags = 1;

    //stdout in position 1
    pcb -> fd_items[STDOUT_IDX].file_op_jmp = stdout_table;
    pcb -> fd_items[STDOUT_IDX].inode = 0;
    pcb -> fd_items[STDOUT_IDX].file_position = 0;
    pcb -> fd_items[STDOUT_IDX].flags = 1;

    return pcb;
}

/*
 * parse_command
 *   DESCRIPTION: helper function parse execute function ,move arguments to pcb for safekeeping
 *   INPUTS: command and pcb_pointer
 *   RETURN VALUE: if successful 1, if fail -1
 */
int parse_command(const uint8_t* command, pcb_t* pcb, int pid){
    uint8_t exec_buf[CMD_MAX_LEN];
    int i;
    int j = 0;
    //exec status stages 0 = nothing, 1 = started, 2 = completed
    uint8_t exec_status = 0;

    for (i = 0; i < TERM_BUF_SIZE; i++) {
        //end of command if newline or null char
        if (command[i] == '\0' || command[i] == '\n'){
            if (i == 0)
                return -1;
            break;
        }

        //at first non-space, char exec starts
        if (command[i] != ' ' && exec_status == 0)
            exec_status = 1;
        //after first space after exec starts, exec ends
        else if (command[i] == ' ' && exec_status == 1)
            exec_status = 2;

        //put characters into correct buffer depending on exec status
        if (command[i] != ' ' && exec_status == 1)
            exec_buf[i] = command[i];
        //argument buffer is space padded
        if (exec_status == 2)
            pcb -> argument_buf[j] = command[i];
            j++;
    }
    //check if file exists
    if (file_open(exec_buf) == -1) 
        return -1;

    //check that first four characters are Delete, E, L, F
    uint8_t first_4_char[4];
    file_read(0, first_4_char, 4);
    if (first_4_char[0] != DELETE || first_4_char[1] != E || first_4_char[2] != L || first_4_char[3] != F)
        return -1;

    return 1;
}

/*
 * read
 *   DESCRIPTION: opens file at given fd for reading into the buffer
 *   INPUTS: file descriptor idx (fd), buffer to read into (buf), number of bytes to read (nbytes)
 *   RETURN VALUE: if successful returns amount of bytes read, if fail -1
 */
int32_t read (int32_t fd, void* buf, int32_t nbytes) {

    // check for invalid conditions
    if (buf == NULL || nbytes < 0 || fd < 0 || fd == STDOUT_IDX || fd > MAX_OPEN_FILES) {
        return -1;
    }

    // get the pcb ptr
    pcb_t* pcb = get_pcb_addr(get_latest_pid());
    fd_items_t file_item = pcb->fd_items[fd];

    // check for write-only
    if (file_item.flags == O_WRONLY) {
        printf("This file is write only. You can't read it.\n");
        return -1;
    }

    return file_item.file_op_jmp.read(fd, buf, nbytes);

}

/*
 * write
 *   DESCRIPTION: writes into the passed in buffer
 *   INPUTS: file descriptor idx (fd), buffer to read into (buf), number of bytes to write (nbytes)
 *   RETURN VALUE: if successful returns 0, if fail -1
 */
int32_t write (int32_t fd, const void* buf, int32_t nbytes) {

    // check for invalid conditions
    if (buf == NULL || nbytes < 0 || fd <= STDIN_IDX || fd > MAX_OPEN_FILES) {
        return -1;
    }

    // get the pcb ptr
    pcb_t* pcb = get_pcb_addr(get_latest_pid());
    fd_items_t file_item = pcb->fd_items[fd];

    // check for read-only
    if (file_item.flags == O_RDONLY) {
        printf("This file is read only. You can't write to it.\n");
        return -1;
    }

    return file_item.file_op_jmp.write(fd, buf, nbytes);
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
