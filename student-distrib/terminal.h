#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.h"

#define VGA_WIDTH 80
#define TERM_BUF_SIZE 128

int32_t terminal_open(const uint8_t* filename);
int32_t terminal_close(int32_t fd);
int32_t terminal_buf_save(unsigned char* buf);
int32_t terminal_read(int32_t fd, unsigned char* buf, int32_t nbytes);
int32_t terminal_write(int32_t fd, unsigned char* buf, int32_t nbytes);

unsigned char terminal_buf[128];

#endif /* TERMINAL_H */
