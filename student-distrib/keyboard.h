#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KB_PORT 0x60
#define KB_IRQ 1

#define SHIFT_PRESS 200
#define CAP_PRESS 201
#define CTRL_PRESS 202
#define SHIFT_RELEASE 203
#define CTRL_RELEASE 204
#define ALT_PRESS 205
#define ALT_RELEASE 206

#define ASCII_L 108
#define KB_SPACE 57
#define TERM_BUF_SIZE 128

void reset_buffer();
void keyboard_init();
void keyboard_handler();
void keyboard_print(int byte);

unsigned char kb_buffer[128];
volatile int term_read_flag;

#endif /* KEYBOARD_H */
