#include "terminal.h"
#include "lib.h"
#include "keyboard.h"

/*
 * terminal_open
 *   INPUTS: filename
 *   OUTPUTS: 0
 *   RETURN VALUE: nothing
 */
int32_t terminal_open(const uint8_t* filename){
    return 0;
}

/*
 * terminal_close
 *   Description: Unused for checkpoint 2.
 *   INPUTS: fd
 *   OUTPUTS: None
 *   RETURN VALUE: 0
 */
int32_t terminal_close(int32_t fd){
    return 0;
}

/*
 * terminal_buf_save
 *   DESCRIPTION: term_buf is now kb_buf
 *   INPUTS:buf
 *   OUTPUTS: 0
 *   RETURN VALUE: term_buf is now kb_buffer
 */
int32_t terminal_buf_save(unsigned char* buf){
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
int32_t terminal_read(int32_t fd, unsigned char* buf, int32_t nbytes){
    // set last char to newline
    term_read_flag = 0;

    while(term_read_flag == 0);

    int i;
    int smallBuf;
    if (nbytes > TERM_BUF_SIZE)
        smallBuf = TERM_BUF_SIZE;
    else 
        smallBuf = nbytes;
    for(i=0; i<(nbytes); i++)
        buf[i] = terminal_buf[i];
    buf[smallBuf - 1] = '\n';
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
int32_t terminal_write(int32_t fd, unsigned char* buf, int32_t nbytes){
    int i;
    int smallBuf;
    if (nbytes > TERM_BUF_SIZE)
        smallBuf = TERM_BUF_SIZE;
    else 
        smallBuf = nbytes;
    for(i=0; i<smallBuf; i++) {
        if(buf[i] != 0)
            putc(buf[i]);
    }
    return 0;
}
