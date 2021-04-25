#include "terminal.h"
#include "lib.h"
#include "keyboard.h"

/*
 * terminal_open
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
        buf[i] = kb_buffer[i];
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
            break;
        }
    }

    return i;
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
 *   DESCRIPTION: sets up the terminal switching
 */
void terminal_switch(int fNumber){
    printf("Switching to terminal ");
    if (fNumber == 10){
        putc(fNumber + NUM_TO_ASCII - 9);
        putc(fNumber + NUM_TO_ASCII - 10);
    }
    else
        putc(fNumber + NUM_TO_ASCII);
    printf("\n");
}
