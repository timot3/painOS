#include "terminal.h"
#include "lib.h"
#include "keyboard.h"

#include "syscall.h"

volatile uint8_t current_terminal = 1;

term_struct_t terminals[MAX_TERMINALS];

/*
 * init_terminals
 *   DESCRIPTION: Inits the terminal structs in the terminals array.
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: 0
 */
int32_t terminals_init() {
    int term_idx, buf_idx;
    term_struct_t* curr_term_struct;
    for (term_idx = 0; term_idx < MAX_TERMINALS; term_idx++) {
        curr_term_struct = &(terminals[term_idx]);
        // set base pid to the idx in array
        curr_term_struct->base_pid = term_idx;
        // no processes are currently running and the terminal is not displayed
        // by default
        curr_term_struct->curr_pid = -1; 
        curr_term_struct->is_active = NOT_ACTIVE;
        // not yet initialized -- set later
        curr_term_struct->cursor_x_pos = NULL;
        curr_term_struct->cursor_y_pos = NULL;
        curr_term_struct->vidmem_start = NULL;

        // clear the buffer
        for (buf_idx = 0; buf_idx < TERM_BUF_SIZE; buf_idx++) {
            curr_term_struct->kb_buf[buf_idx] = 0;
        }

    }
    printf("Finished initing %d terminals.\n", MAX_TERMINALS); 
    return 0;
}
/*
 * terminal_open
 *   Description: Unused for checkpoint 2.
 *   INPUTS: filename
 *   OUTPUTS: -1
 *   RETURN VALUE: nothing
 */
int32_t terminal_open(const uint8_t* filename) {
    return -1;
}

/*
 * terminal_close
 *   Description: Unused for checkpoint 2.
 *   INPUTS: fd
 *   OUTPUTS: -1
 *   RETURN VALUE: 0
 */
int32_t terminal_close(int32_t fd) {
    return -1;
}

/*
 * terminal_buf_save
 *   DESCRIPTION: term_buf is now kb_buf
 *   INPUTS:buf
 *   OUTPUTS: 0
 *   RETURN VALUE: term_buf is now kb_buffer
 */
int32_t terminal_buf_save(unsigned char* buf) {
    int i;
    for(i=0; i<TERM_BUF_SIZE; i++)
        buf[i] = true_buffer[i];
    // buf[TERM_BUF_SIZE - 1] = '\n';
    return 0;
}

/*
 * terminal_read
 *   DESCRIPTION: set buf to terminal buffer
 *   INPUTS: fd, buf, nybtes
 *   OUTPUTS: none
 *   RETURN VALUE: 0
 */
int32_t terminal_read(int32_t fd, void *buf, int32_t nbytes) {
    // set last char to newline
    memset(terminal_buf, 0, TERM_BUF_SIZE);
    memset(buf, 0, nbytes);
    term_read_flag = 0;
    sti();

    while(term_read_flag == 0);
    cli();

    int i;
    int smallBuf;
    if (nbytes > TERM_BUF_SIZE)
        smallBuf = TERM_BUF_SIZE;
    else
        smallBuf = nbytes;
    for(i=0; i<smallBuf; i++) {
        ((uint8_t*)buf)[i] = terminal_buf[i];
        if (((uint8_t*)buf)[i] == '\n') {
            return i+1;
        }
    }

    return -1;
}

/*
 * terminal_write
 *   DESCRIPTION: write buf to screen
 *   INPUTS: fd - currently unused,
 *           buf - buffer to write,
 *           nbytes - how many bytes to write
 *   OUTPUTS: contents of buf
 *   RETURN VALUE: screen is now buf
 *   SIDE EFFECTS: None
 */
int32_t terminal_write(int32_t fd, const void *buf, int32_t nbytes) {
    int i;

    for(i=0; i < nbytes; i++) {
        if(((uint8_t*)buf)[i] != NULL)
            putc(((uint8_t*)buf)[i]);
    }
    return 0;
}

/*
 * std_bad_call
 *   DESCRIPTION: returns fail
 *   RETURN VALUE: -1
 */
int32_t std_bad_call(){
    return -1;
}

/*
 * terminal_switch
 *   DESCRIPTION: goes to display switch lol
 */
void terminal_switch(uint8_t fNumber){
    display_switch(fNumber);
}

/*
 * display_switch
 *   DESCRIPTION: sets up for switch_screen
 */
void display_switch(uint8_t newDisplay){
    if (current_terminal == newDisplay)
        return;

    // get current terminal
    // set it to not actiev
    term_struct_t* old_term = get_active_terminal();
    old_term->is_active = NOT_ACTIVE;

    current_terminal = newDisplay;

    term_struct_t* new_term = get_active_terminal();
    new_term->is_active = ACTIVE;

    switch_screen(old_term, new_term);
    printf("New display: %d\n", newDisplay);
    int pid = get_latest_pcb()->pid;
    printf("pid: %d\n", pid);

}

/*
 * get_current_terminal_idx
 *   DESCRIPTION: our terminal idx is 1-indexed but we need it to be 
 *      0-indexed for array accesses
 *   RETURNS: 0-indexed terminal idx, or 0 on fail
 */
uint8_t get_current_terminal_idx() {
    if (current_terminal > 0 && current_terminal < MAX_TERMINALS + 1) {
        return current_terminal - 1;
    }
    printf("\n\nError getting current terminal ID!! (id: %d) \n\n", current_terminal);
    return TERMINAL_ERROR;
}

/*
 * get_active_terminal
 *   DESCRIPTION: get pointer to currently active terminal struct
 *   RETURNS: 0-indexed terminal idx, or 0 on fail
 */
term_struct_t* get_active_terminal() {
    int term_idx = get_current_terminal_idx();
    if (term_idx == TERMINAL_ERROR) {
        printf("\nFailed to get active terminal. Term idx = %d\n", term_idx);
        return (term_struct_t*)NULL;
    }
    return &(terminals[term_idx]);
}
