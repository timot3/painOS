#include "terminal.h"
#include "lib.h"
#include "keyboard.h"

/*
 * terminal_open
 *   INPUTS: filename
 *   OUTPUTS: 0
 *   RETURN VALUE: nothing
 */
int terminal_open(const int* filename){
    return 0;
}

/*
 * terminal_close
 *   Description: Unused for checkpoint 2.
 *   INPUTS: fd
 *   OUTPUTS: None
 *   RETURN VALUE: 0
 */
int terminal_close(int fd){
    return 0;
}

/*
 * terminal_buf_save
 *   DESCRIPTION: term_buf is now kb_buf
 *   INPUTS:buf
 *   OUTPUTS: 0
 *   RETURN VALUE: term_buf is now kb_buffer
 */
int terminal_buf_save(unsigned char* buf){
    int i;
    for(i=0; i<TERM_BUF_SIZE; i++)
        buf[i] = kb_buffer[i];
    return 0;
}

/*
 * terminal_read
 *   DESCRIPTION: set buf to terminal buffer
 *   INPUTS: fd, buf, nybtes
 *   OUTPUTS: none
 *   RETURN VALUE: 0
 */
int terminal_read(int fd, unsigned char* buf, int nbytes){
    // set last char to newline
    terminal_buf[TERM_BUF_SIZE - 1] = '\n';
    term_read_flag = 0;

    if(nbytes < TERM_BUF_SIZE)
        return -1;

    while(term_read_flag == 0);

    int i;
    for(i=0; i<TERM_BUF_SIZE; i++)
        buf[i] = terminal_buf[i];
    return 0;
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
int terminal_write(int fd, unsigned char* buf, int nbytes){
    int i;
    for(i=0; i<nbytes; i++) {
        putc(buf[i]);
    }
    return 0;
}
