#include "keyboard.h"
#include "lib.h"
#include "i8259.h"
#include "terminal.h"

/*https://wiki.osdev.org/PS/2_Keyboard */
unsigned char scan_code_1[256] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', CTRL_PRESS,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', SHIFT_PRESS, '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/', SHIFT_PRESS, '*', ALT_PRESS, ' ', CAP_PRESS, F1_PRESS,
    F2_PRESS, F3_PRESS, F4_PRESS, F5_PRESS, F6_PRESS, F7_PRESS, F8_PRESS, F9_PRESS, F10_PRESS, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    CTRL_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, SHIFT_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    SHIFT_RELEASE, 0, ALT_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

//same order as scan_code_1 but all keys are their shifted values
unsigned char scan_code_shift[256] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*',
    '(', ')', '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R',
    'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', CTRL_PRESS,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
    '"', '~', SHIFT_PRESS, '|', 'Z', 'X', 'C', 'V', 'B', 'N',
    'M', '<', '>', '?', SHIFT_PRESS, '*', ALT_PRESS, ' ', CAP_PRESS, F1_PRESS,
    F2_PRESS, F3_PRESS, F4_PRESS, F5_PRESS, F6_PRESS, F7_PRESS, F8_PRESS, F9_PRESS, F10_PRESS, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    CTRL_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, SHIFT_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    SHIFT_RELEASE, 0, ALT_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

//same order as scan_code_1 but all keys are their caps lock values
unsigned char scan_code_caps[256] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b', '\t', 'Q', 'W', 'E', 'R',
    'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', CTRL_PRESS,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',
    '\'', '`', SHIFT_PRESS, '\\', 'Z', 'X', 'C', 'V', 'B', 'N',
    'M', ',', '.', '/', SHIFT_PRESS, '*', ALT_PRESS, ' ', CAP_PRESS, F1_PRESS,
    F2_PRESS, F3_PRESS, F4_PRESS, F5_PRESS, F6_PRESS, F7_PRESS, F8_PRESS, F9_PRESS, F10_PRESS, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    CTRL_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, SHIFT_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    SHIFT_RELEASE, 0, ALT_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
//same order as scan_code_1 but all keys are their shifted caps lock values
unsigned char scan_code_shift_caps[256] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*',
    '(', ')', '_', '+', '\b', '\t', 'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n', CTRL_PRESS,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':',
    '"', '~', SHIFT_PRESS, '|', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', '<', '>', '?', SHIFT_PRESS, '*', ALT_PRESS, ' ', CAP_PRESS, F1_PRESS,
    F2_PRESS, F3_PRESS, F4_PRESS, F5_PRESS, F6_PRESS, F7_PRESS, F8_PRESS, F9_PRESS, F10_PRESS, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    CTRL_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, SHIFT_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    SHIFT_RELEASE, 0, ALT_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

//keyboard buffer to store values for terminal_read
unsigned char kb_buffer[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

unsigned char kb_buffer2[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

unsigned char kb_buffer3[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

unsigned char kb_buffer4[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

unsigned char kb_buffer5[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

unsigned char kb_buffer6[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

unsigned char kb_buffer7[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

unsigned char *true_buffer = kb_buffer;

// current location in the terminal buffer
int term_buf_1 = 0;
int term_buf_2 = 0;
int term_buf_3 = 0;
int term_buf_4 = 0;
int term_buf_5 = 0;
int term_buf_6 = 0;
int term_buf_7 = 0;
int *term_buf_location = &term_buf_1;

/*
 * reset_buffer
 *   DESCRIPTION: Reset the keyboard buffer
 *   INPUTS: none
 *   OUTPUTS: resets the keyboard buffer
 *   RETURN VALUE: none
 */
void reset_buffer(){
    int i;
    for (i=0; i<127; i++){
        true_buffer[i] = 0;
    }
}

//flag if button is in use
char shift_flag = 0;
char cap_flag = 0;
char ctrl_flag = 0;
char alt_flag = 0;

/*
 * keyboard_init
 *   DESCRIPTION: Initialize the keyboard
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
void keyboard_init() {
    // set the current terminal
    enable_irq(KB_IRQ);
}

/*
 * keyboard_handler
 *   DESCRIPTION: Runs every time an interrupt is created by the keyboard.
 *                  Takes character input and does initial work before passing to keyboard_print
 *   INPUTS: none
 *   OUTPUTS: character values for keyboard_print
 *   RETURN VALUE: none
 */
void keyboard_handler() {
    cli();
    unsigned int byte = inb(KB_PORT);
    unsigned char c = scan_code_1[byte];
    int i;
    //look through all special key cases and assign correct flags or special features
    switch(c){
        //only allow keys with function through
        case 0:
            break;
        case ALT_PRESS:
        case ALT_RELEASE:
            alt_flag = alt_flag ^ 1;
            break;
        case CAP_PRESS:
            cap_flag = cap_flag ^ 1;
            break;
        case SHIFT_PRESS:
        case SHIFT_RELEASE:
            shift_flag = shift_flag ^ 1;
            break;
        case CTRL_PRESS:
        case CTRL_RELEASE:
            ctrl_flag = ctrl_flag ^ 1;
            break;
        //tabs = 4 spaces
        case '\t':
            for(i=0; i<4; i++){
                keyboard_print(KB_SPACE);
            }
            break;
        default:
            keyboard_print(byte);
    }
    send_eoi(KB_IRQ);
}

/*
 * keyboard_print
 *   DESCRIPTION: prints out the correct response based on keyboard input
 *   INPUTS: byte
 *   OUTPUTS: text to screen
 *   RETURN VALUE: none
 */
void keyboard_print(int byte) {
    unsigned char c;
    //ctrl+L = clear
    if (ctrl_flag == 1 && scan_code_1[byte] == ASCII_L){
        clear();
        terminal_write(0, true_buffer, 128);
        return;
    }
    //switch to correct terminal given alt + F#
    else if (alt_flag == 1 && scan_code_1[byte] == F1_PRESS){
        terminal_switch(1);
        true_buffer = kb_buffer;
        term_buf_location = &term_buf_1;
        return;
    }
    else if (alt_flag == 1 && scan_code_1[byte] == F2_PRESS){
        terminal_switch(2);
        true_buffer = kb_buffer2;
        term_buf_location = &term_buf_2;
        return;
    }
    else if (alt_flag == 1 && scan_code_1[byte] == F3_PRESS){
        terminal_switch(3);
        true_buffer = kb_buffer3;
        term_buf_location = &term_buf_3;
        return;
    }
    else if (alt_flag == 1 && scan_code_1[byte] == F4_PRESS){
        terminal_switch(4);
        true_buffer = kb_buffer4;
        term_buf_location = &term_buf_4;
        return;
    }
    else if (alt_flag == 1 && scan_code_1[byte] == F5_PRESS){
        terminal_switch(5);
        true_buffer = kb_buffer5;
        term_buf_location = &term_buf_5;
        return;
    }
    else if (alt_flag == 1 && scan_code_1[byte] == F6_PRESS){
        terminal_switch(6);
        true_buffer = kb_buffer6;
        term_buf_location = &term_buf_6;
        return;
    }
    else if (alt_flag == 1 && scan_code_1[byte] == F7_PRESS){
        terminal_switch(7);
        true_buffer = kb_buffer7;
        term_buf_location = &term_buf_7;
        return;
    }
    //dont print out garbage F# input
    else if (scan_code_1[byte] >= F1_PRESS && scan_code_1[byte] <= F10_PRESS)
        return;
    //correct correct scancode table depending on shift/tab
    else if (cap_flag == 1 && shift_flag == 1)
        c = scan_code_shift_caps[byte];
    else if (cap_flag == 1 && shift_flag == 0)
        c = scan_code_caps[byte];
    else if (cap_flag == 0 && shift_flag == 1)
        c = scan_code_shift[byte];
    else
        c = scan_code_1[byte];

    //get correct backspace behavior
    if (scan_code_1[byte] == '\b'){
        if(*term_buf_location > 0){
            delete();
            // reset current char in keyboard buffer
            // decrement current buffer location
            (*term_buf_location)--;
            true_buffer[*term_buf_location] = 0;
        }
        return;
    }

    //get correct newline behavior
    if (c == '\n' || c == '\r'){
        true_buffer[*term_buf_location] = c;
        terminal_buf_save(terminal_buf);
        *term_buf_location = 0;
        term_read_flag = 1;
        reset_buffer();
        putc(c);
        return;
    }
    if (*term_buf_location >= TERM_BUF_SIZE - 1)
        return;

    putc(c);
    true_buffer[*term_buf_location] = c;
    (*term_buf_location)++;
}
