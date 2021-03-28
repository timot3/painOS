#ifndef TERMINAL_H
#define TERMINAL_H

#define VGA_WIDTH 80
#define TERM_BUF_SIZE 128

int terminal_open(const int* filename);
int terminal_close(int fd);
int terminal_read(int fd, unsigned char* buf, int nbytes);
int terminal_write(int fd, unsigned char* buf, int nbytes);
int terminal_buf_save(unsigned char* buf);

unsigned char terminal_buf[128];

#endif /* TERMINAL_H */
