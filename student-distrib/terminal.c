#include "terminal.h"
#include "lib.h"
#include "keyboard.h"

unsigned char term_buf[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '\n'
    };

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
 *   DESCRIPTION: buf is now term_buf
 *   INPUTS: fd, buf, nybtes
 *   OUTPUTS: 0
 *   RETURN VALUE: buf is now term_buf
 */
int terminal_read(int fd, unsigned char* buf, int nbytes){
    term_read_flag = 0;

    if(nbytes < TERM_BUF_SIZE)
        return -1;
        
    while(term_read_flag == 0){

    }  
    int i;
    for(i=0; i<TERM_BUF_SIZE; i++)
        buf[i] = term_buf[i];
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
    if(buf[0] == 0)
        return 0;
    for(i=0; i<TERM_BUF_SIZE; i++)
        putc(buf[i]);
    return 0;
}

