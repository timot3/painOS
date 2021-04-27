#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.h"

#define VGA_WIDTH 80
#define TERM_BUF_SIZE 128
#define NUM_TO_ASCII 48

#define MAX_TERMINALS 3
#define CURRENT_TERMINAL (current_terminal - 1)

int32_t terminal_open(const uint8_t *filename);
int32_t terminal_close(int32_t fd);
int32_t terminal_buf_save(unsigned char *buf);
int32_t terminal_read(int32_t fd, void *buf, int32_t nbytes);
int32_t terminal_write(int32_t fd, const void *buf, int32_t nbytes);

int32_t std_bad_call();

void terminal_switch(uint8_t fNumber);
void display_switch(uint8_t newDisplay);

uint8_t get_current_terminal_idx();

unsigned char terminal_buf[128];
extern volatile uint8_t current_terminal;

#endif /* TERMINAL_H */
