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
 *   INPUTS: fd
 *   OUTPUTS: 0
 *   RETURN VALUE: nothing
 */
int terminal_close(int fd){
    return 0;
}

/*
 * terminal_read
 *   DESCRIPTION: buf is now kb_buf
 *   INPUTS: fd, buf, nybtes
 *   OUTPUTS: 0
 *   RETURN VALUE: buf is now kb_buffer
 */
int terminal_read(int fd, unsigned char* buf, int nbytes){
    buf = kb_buffer;
    return 0;
}

/*
 * terminal_write
 *   DESCRIPTION: write buf to screen
 *   INPUTS: fd, buf, nbytes
 *   OUTPUTS: 0
 *   RETURN VALUE: screen is now buff
 */
int terminal_write(int fd, unsigned char* buf, int nbytes){
    int i;
    for(i=0; i<nbytes; i++)
        putc(buf[i]);
    return 0;
}

