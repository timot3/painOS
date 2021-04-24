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
    CTRL_RELEASE, F1_RELEASE, F2_RELEASE, F3_RELEASE, F4_RELEASE, F5_RELEASE, F6_RELEASE, F7_RELEASE, F8_RELEASE, F9_RELEASE, F10_RELEASE,
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
    CTRL_RELEASE, F1_RELEASE, F2_RELEASE, F3_RELEASE, F4_RELEASE, F5_RELEASE, F6_RELEASE, F7_RELEASE, F8_RELEASE, F9_RELEASE, F10_RELEASE,
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
    CTRL_RELEASE, F1_RELEASE, F2_RELEASE, F3_RELEASE, F4_RELEASE, F5_RELEASE, F6_RELEASE, F7_RELEASE, F8_RELEASE, F9_RELEASE, F10_RELEASE,
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
    CTRL_RELEASE, F1_RELEASE, F2_RELEASE, F3_RELEASE, F4_RELEASE, F5_RELEASE, F6_RELEASE, F7_RELEASE, F8_RELEASE, F9_RELEASE, F10_RELEASE,
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


// current location in the terminal buffer
int term_buf_location = 0;

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
        kb_buffer[i] = 0;
    }
}

//flag if button is in use
char shift_flag = 0;
char cap_flag = 0;
char ctrl_flag = 0;
char alt_flag = 0;
char f1_flag = 0;
char f2_flag = 0;
char f3_flag = 0;
char f4_flag = 0;
char f5_flag = 0;
char f6_flag = 0;
char f7_flag = 0;
char f8_flag = 0;
char f9_flag = 0;
char f10_flag = 0;




/*
 * keyboard_init
 *   DESCRIPTION: Initialize the keyboard
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
void keyboard_init() {
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
        case F1_PRESS:
        case F1_RELEASE:
            f1_flag = f1_flag ^ 1;
            break;
        case F2_PRESS:
        case F2_RELEASE:
            f2_flag = f2_flag ^ 1;
            break;
        case F3_PRESS:
        case F3_RELEASE:
            f3_flag = f3_flag ^ 1;
            break;
        case F4_PRESS:
        case F4_RELEASE:
            f4_flag = f4_flag ^ 1;
            break;
        case F5_PRESS:
        case F5_RELEASE:
            f5_flag = f5_flag ^ 1;
            break;
        case F6_PRESS:
        case F6_RELEASE:
            f6_flag = f6_flag ^ 1;
            break;
        case F7_PRESS:
        case F7_RELEASE:
            f7_flag = f7_flag ^ 1;
            break;
        case F8_PRESS:
        case F8_RELEASE:
            f8_flag = f8_flag ^ 1;
            break;
        case F9_PRESS:
        case F9_RELEASE:
            f9_flag = f9_flag ^ 1;
            break;
        case F10_PRESS:
        case F10_RELEASE:
            f10_flag = f10_flag ^ 1;
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
        terminal_write(0, kb_buffer, 128);
        return;
    }
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
        if(term_buf_location > 0){
            delete();
            // reset current char in keyboard buffer
            // decrement current buffer location
            term_buf_location--;
            kb_buffer[term_buf_location] = 0;
        }
        return;
    }

    //get correct newline behavior
    if (c == '\n' || c == '\r'){
        kb_buffer[term_buf_location] = c;
        terminal_buf_save(terminal_buf);
        term_buf_location = 0;
        term_read_flag = 1;
        reset_buffer();
        putc(c);
        return;
    }
    if (term_buf_location >= TERM_BUF_SIZE - 1)
        return;

    putc(c);
    kb_buffer[term_buf_location] = c;
    term_buf_location++;
}
