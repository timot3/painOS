#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.h"

#define VGA_WIDTH 80
#define TERM_BUF_SIZE 128
#define NUM_TO_ASCII 48

#define MAX_TERMINALS 3
#define NOT_ACTIVE 0
#define ACTIVE 1

// error code for terminal-related stuff (used for debugging)
// 19 is chosen arbitrarily because it is large enough to not be an issue
// and also is a cool number
#define TERMINAL_ERROR 19

typedef struct term_struct {
    int8_t base_pid; // 0, 1, or 2
    int8_t curr_pid; // pid of currently running process, -1 if none
    uint8_t is_active; // if it's the current terminal, either 0 or 1
    uint8_t** vidmem_start; // address of vidmem
    int32_t cursor_x_pos;
    int32_t cursor_y_pos;
    uint8_t kb_buf[TERM_BUF_SIZE];
    int buf_location;
} term_struct_t;

int32_t terminals_init();
int32_t terminal_open(const uint8_t *filename);
int32_t terminal_close(int32_t fd);
int32_t terminal_buf_save(unsigned char *buf);
int32_t terminal_read(int32_t fd, void *buf, int32_t nbytes);
int32_t terminal_write(int32_t fd, const void *buf, int32_t nbytes);

int32_t std_bad_call();

void terminal_switch(uint8_t fNumber);
void terminal_switch_not_visual(uint8_t newTerm);
void display_switch(uint8_t newDisplay);

term_struct_t* get_active_terminal();
uint8_t get_current_terminal_idx();

unsigned char terminal_buf[128];
extern volatile uint8_t current_terminal;

#endif /* TERMINAL_H */
