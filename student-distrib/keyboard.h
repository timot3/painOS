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
#define F1_PRESS 207
#define F2_PRESS 208
#define F3_PRESS 209
#define F4_PRESS 210
#define F5_PRESS 211
#define F6_PRESS 212
#define F7_PRESS 213
#define F8_PRESS 214
#define F9_PRESS 215
#define F10_PRESS 216

#define ASCII_L 108
#define KB_SPACE 57
#define TERM_BUF_SIZE 128

#define MAX_TERMINALS 3

void reset_buffer();
void keyboard_init();
void keyboard_handler();
void keyboard_print(int byte);

unsigned char kb_buffer[128];
unsigned char *true_buffer;
volatile int term_read_flag;

#endif /* KEYBOARD_H */
