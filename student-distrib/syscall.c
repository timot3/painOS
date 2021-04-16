#include "syscall.h"
#include "filesys.h"
volatile uint8_t ret_status = -1;

static file_op_table_t rtc_table = {
    .open = rtc_open,
    .read = rtc_read,
    .write = rtc_write,
    .close = rtc_close
};

static file_op_table_t file_table = {
    .open = file_open,
    .read = file_read,
    .write = file_write,
    .close = file_close
};

static file_op_table_t dir_table = {
    .open = dir_open,
    .read = dir_read,
    .write = dir_write,
    .close = dir_close
};

static file_op_table_t stdin_table = {
    .open = std_bad_call,
    .read = terminal_read,
    .write = std_bad_call,
    .close = std_bad_call
};

static file_op_table_t stdout_table = {
    .open = std_bad_call,
    .read = std_bad_call,
    .write = terminal_write,
    .close = std_bad_call
};

char pid_arr[PROCESS_LIMIT] = {0, 0};

int32_t curr_pid = 0;

int32_t halt (uint8_t status) {
    int i;
    // get current pcb
    pcb_t* pcb = get_pcb_addr(get_latest_pid());
    pcb_t* parent = get_pcb_addr(pcb->parent.pid);
    if (pcb == NULL) return -1;
    // 1.  if base shell, re-execute base shell
    if (pcb->pid == parent->pid) {
        // 7 is the length of our string
        execute((uint8_t*)"shell");
    }
    //else:
    //2.  close all file descriptors
    for (i = 0; i < MAX_OPEN_FILES; i++) {
        fd_items_t curr_fd_item = pcb->fd_items[i];
        // 3rd bit of flag is the "not in use" bit
        // so bitwise anding it with 0x4 == 0b100 will
        // check if file is in use
        if ((curr_fd_item.flags & 0x4) != 0) {
            // mark file as not present
            curr_fd_item.flags ^= 0x4;
            // call close on file
          //  curr_fd_item.file_op_jmp.close(i);
        }
    }
    // 3.  set up file state for return to parent

    // unassign current pid
    unassign_pid(pcb->pid);
    // instead assign it to the parent pid
    pcb->pid = pcb->parent.pid;

    // 4.  restore parent's paging and flush the tlb
    map_page_pid(pcb->pid);
    
    // assembly wrapper for tlb flush
    tlb_flush();

    // 5.  set tss to point to parent's stack
    // set esp0 to point to base of parent's kernel stack
    tss.esp0 = pcb->parent.kbp;
    // set ss0 to kernel data segment
    tss.ss0 = KERNEL_DS;

    // 6.  swap to saved parent's stack state and return from execute
    asm volatile (
        "movl %0, %%eax;"
            : 
            : "g" (status)
            : "eax"
    );

    asm volatile ("jmp execute_return");
    // must return with value of 256
    // ret_status = status;
    return 256;

}
/*
 * execute
 *   DESCRIPTION: executs the command, lots of helper functions see them for more details
 *   INPUTS: command
 *   RETURN VALUE:
 */
int32_t execute (const uint8_t* command) {
    //get pid if none avaialbe fail
    int pid = assign_pid();
    if (pid == -1) return -1;

    //get pcb and initialize
    dentry_t dentry;
    pcb_t* pcb = allocate_pcb(pid);
    int parse = parse_command(command, pcb, pid, &dentry);

    //parse command, if bad fail
    if (parse == -1){
        unassign_pid(pid);
        return -1;
    }

    //set up page
    map_page_pid(pid);

    //copy user program to page

    read_data(dentry.inode, 0, (uint8_t*)BUFFER_START, MAX_FILE_SIZE);


    //setup TSS for good context switching
    setup_TSS(pid);

    //save current stack state
    int ksp;
    asm volatile(
        "movl %%esp, %0": "=r" (ksp)
    );

    int kbp;
    asm volatile(
        "movl %%ebp, %0": "=r" (kbp)
    );

    pcb -> parent.ksp = ksp;
    pcb -> parent.kbp = kbp;

    //get starting EIP at 24 bytes
    uint8_t EIPbuf[4];
    read_data(dentry.inode, 24, EIPbuf, 4);
    uint32_t EIP = *((uint32_t*)EIPbuf);

    //iret context switch, set EIP, CS, flags (set interrupt flag manually), user stack address, ss
    asm volatile(
        "pushl %0;"
        "pushl %1;"
        "pushfl;"
        "popl %%eax;"
        "orl $0x200, %%eax;"
        "pushl %%eax;"
        "pushl %2;"
        "pushl %3;"
        "iret;" 
        :
        : "g" (USER_DS), "g" (USER_PAGE_BOT), "g" (USER_CS), "g" (EIP)
    );

    asm volatile ("execute_return:");
    return 0;
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
            curr_pid = i;
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
    return curr_pid;
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

    // set parent to null
    pcb->parent.ksp = 0;
    pcb->parent.kbp = 0;


    pcb -> pid = pid;
    return pcb;
}

/*
 * parse_command
 *   DESCRIPTION: helper function parse execute function ,move arguments to pcb for safekeeping
 *   INPUTS: command and pcb_pointer
 *   RETURN VALUE: if successful 1, if fail -1
 */
int parse_command(const uint8_t* command, pcb_t* pcb, int pid, dentry_t *dentry) {
    uint8_t exec_buf[CMD_MAX_LEN];
    int i;
    int j = 0;
    //exec status stages 0 = nothing, 1 = started, 2 = completed
    uint8_t exec_status = 0;

    for (i = 0; i < TERM_BUF_SIZE; i++) {
        //end of command if newline or null char
        if (command[i] == '\0' || command[i] == '\n') {
            if (i == 0)
                return -1;

            // Add null terminating ending
            exec_buf[i] = '\0';
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
    // Clear parts of exec_buf not used -> if not done file_open returns -1
    for(i = strlen((const int8_t*)exec_buf) + 1; i < CMD_MAX_LEN; i++)
        exec_buf[i] = 0;
    if (file_open((const uint8_t*)exec_buf) == -1)
        return -1;

    //check that first four characters are Delete, E, L, F
    uint8_t first_4_char[4];
    file_read(0, first_4_char, 4);
    if (first_4_char[0] != DELETE || first_4_char[1] != E || first_4_char[2] != L || first_4_char[3] != F)
        return -1;

    read_dentry_by_name(exec_buf, dentry);

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
    if ((file_item.flags & READ_WRITE_MASK) == O_RDONLY) {
        printf("This file is read only. You can't write to it.\n");
        return -1;
    }

    return file_item.file_op_jmp.write(fd, buf, nbytes);
}

void setup_TSS(int pid) {
    //ss0 at kernel data segment
    tss.ss0 = KERNEL_DS;
    //esp0 at base of k stack
    tss.esp0 = KERNEL_PAGE_BOT - KERNEL_STACK_SIZE * pid;
}

/*
 * syscall open
 *   DESCRIPTION: Opens a file
 *   INPUTS: none
 *   OUTPUTS: None
 *   RETURN VALUE: None
 *   Side Effects:
 * * Allocates fda entry for fname
 * * * find free entry
 * * * Setup fops
 * * * inode
 * * * file_pos
 * * * flags
 * * calls fop fopen
 * * returns fd
 */
int32_t open (const uint8_t* filename) {
    pcb_t* pcb = (pcb_t*)(KERNEL_PAGE_BOT - (curr_pid + 1) * KERNEL_STACK_SIZE);
    dentry_t tmp_dentry;
    file_op_table_t* tmp_f_ops;
    int i = 0, asm_ret_val;
    // check if valid name
    // check null
    if (filename[0] == '\0'){
        return -1;
    }
    // check len
    if (strlen((int8_t*)filename) > 33){
        return -1;
    }
    // handle stdin
    if (strncmp((int8_t*)filename, (int8_t*)"stdin", 5)){
        // stdin is index0
        pcb->fd_items[0].file_op_jmp = stdin_table;
        if(set_active_flag(0, ACTIVE_FLAG))
            return 0; // return fd
        else
            return -1; // failed
    }


    // handle stdout
    if (strncmp((int8_t*)filename, (int8_t*)"stdout", 6)){
        // stdout is index1
        pcb->fd_items[1].file_op_jmp = stdout_table;
        if (set_active_flag(1, ACTIVE_FLAG))
            return 1; // return fd
        else
            return -1; // failed
    }


    // check if file exists
    if (read_dentry_by_name(filename, &tmp_dentry) == -1){
        return -1;
    }
    // dentry info is in tmp_dentry

    // check for open spot in pcb
    for (i=0; i<MAX_OPEN_FILES; i++){
        if (i==0 || i==1)
            continue;
        if ((pcb->fd_items[i].flags & ACTIVE_FLAG_MASK) == 0) // check if active
            break;
    }
    // i stores open spot


    // grab fops
    if(tmp_dentry.type == 0) // dentry type 0 is RTC
        tmp_f_ops = &rtc_table;
    else if (tmp_dentry.type == 1) // dentry type 1 is directory
        tmp_f_ops = &dir_table;
    else if (tmp_dentry.type == 2) // dentry type 2 is file
        tmp_f_ops = &file_table;
    else  // no other supported dentry types
        return -1;

    // call fopen, trying with asm first
    asm volatile("pushl	%2;"
				 "call  *%1;"
		 		 "movl 	%%eax,%0;"
		 		 "addl	$4,%%esp;"
		 		 : "=r"(asm_ret_val)
		 		 : "g" ((*tmp_f_ops).open), "g" (filename)
		 		 : "eax");

    if (asm_ret_val == -1)
        return -1;
    // if success set struct data - set flag to active (3rd bit (& 4) == 1 )
    if (!set_active_flag(i, ACTIVE_FLAG))
        return -1; // failed
    pcb->fd_items[i].file_op_jmp = *tmp_f_ops;
    pcb->fd_items[i].inode = tmp_dentry.inode;
    pcb->fd_items[i].file_position = 0;
    return i;
}

/*
set_active_flag
DESCRIPTION: Sets fd entry to the new status passed in.
iNPUTS: fd -- file descriptor, new_status -- new status to set to
OUTPUTS: none
SIDE EFFECTS: changes fd's flag to new status
RETURNS: 0 on fail, 1 on success.
*/
int32_t set_active_flag (int32_t fd, int32_t new_status){
    pcb_t* pcb = (pcb_t*)(KERNEL_PAGE_BOT - (curr_pid + 1) * KERNEL_STACK_SIZE);
    uint32_t flags = pcb->fd_items[fd].flags;
    if (new_status == 1){  // if setting to active
        if ((flags & ACTIVE_FLAG_MASK) > 0)
            return 0;
        else{
            flags |= ACTIVE_FLAG_MASK;
        }
    }
    else if (new_status == 0){
        if ((flags & ACTIVE_FLAG_MASK) == 0)
            return 0;
        else{
            flags ^= ACTIVE_FLAG_MASK; // ==4
        }
    }
    return 1; // success
}
/*
 * syscall close
 *   DESCRIPTION: close a file
 *   INPUTS: none
 *   OUTPUTS: None
 *   RETURN VALUE: None
 *   Side Effects:
 * * Unallocates fda entry for fname
 * * calls fop fclose
 * * returns fd
 */

int32_t close (int32_t fd) {
    pcb_t* pcb = (pcb_t*)(KERNEL_PAGE_BOT - (curr_pid + 1) * KERNEL_STACK_SIZE);

    int i = 0, asm_ret_val;
    // dont let them close stdin/out
    if(fd == 0 || fd == 1)
        return -1;

    // bounds check
    if(fd < 0 || fd >= MAX_OPEN_FILES)
        return -1;

    // dont let them close inactive files
    if((pcb->fd_items[fd].flags & ACTIVE_FLAG_MASK) == 0)
        return -1;


    // call fclose, handle failed fclose
    asm volatile("pushl	%2;"
				 "call  *%1;"
		 		 "movl 	%%eax,%0;"
		 		 "addl	$4,%%esp;"
		 		 : "=r"(asm_ret_val)
		 		 : "g" (pcb->fd_items[fd].file_op_jmp.close), "g" (fd)
		 		 : "eax");

    // check if successful
    if (asm_ret_val == -1)
        return -1;

    // set flag to free
    if (!set_active_flag(i, INACTIVE_FLAG))
        return -1; // failed

    pcb->fd_items[fd].inode = -1;
    pcb->fd_items[fd].file_position = -1;
    return 0;
}
/*
getargs
DESCRIPTION: gets args from buffer
iNPUTS: buf -- buffer, nbytes -- number of bytes to read
OUTPUTS: none
SIDE EFFECTS: none --not yet implemented 
RETURNS: -1
*/
int32_t getargs (uint8_t* buf, int32_t nbytes) {
    return -1;

}
/*
vidmap
DESCRIPTION: not yet implemented
iNPUTS: not yet implemented
OUTPUTS: not yet implemented
SIDE EFFECTS: not yet implemented
RETURNS: -1
*/
int32_t vidmap (uint8_t** screen_start) {
    return -1;

}
/*
set_handler
DESCRIPTION: not yet implemented
iNPUTS: not yet implemented
OUTPUTS: not yet implemented
SIDE EFFECTS: not yet implemented
RETURNS: -1
*/
int32_t set_handler (int32_t signum, void* handler_address) {
    return -1;

}
/*
sigreturn 
DESCRIPTION: not yet implemented
iNPUTS: not yet implemented
OUTPUTS: not yet implemented
SIDE EFFECTS: not yet implemented
RETURNS: -1
*/
int32_t sigreturn (void) {
    return -1;

}
