#include "keyboard.h"
#include "lib.h"
#include "i8259.h"
#include "terminal.h"

/* 0's in order stand in for nothing, escape, enter
capslock, F1 - F10, numberlock, scrolllock
200's in order stand for left-shift pressed, right-shift pressed
201 stands for caps-lock pressed
202 stands for left-ctrl pressed
203's in order stand for left-shift released, right-shift released
204 stands for left-ctrl released
205 stands for left-alt pressed
https://wiki.osdev.org/PS/2_Keyboard */
unsigned char scan_code_1[256] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', CTRL_PRESS,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', SHIFT_PRESS, '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/', SHIFT_PRESS, '*', ALT_PRESS, ' ', CAP_PRESS, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3',
    '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    CTRL_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SHIFT_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
    'M', '<', '>', '?', SHIFT_PRESS, '*', ALT_PRESS, ' ', CAP_PRESS, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3',
    '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    CTRL_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SHIFT_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
    'M', ',', '.', '/', SHIFT_PRESS, '*', ALT_PRESS, ' ', CAP_PRESS, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3',
    '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    CTRL_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SHIFT_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
    'm', '<', '>', '?', SHIFT_PRESS, '*', ALT_PRESS, ' ', CAP_PRESS, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3',
    '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    CTRL_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SHIFT_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    SHIFT_RELEASE, 0, ALT_RELEASE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };


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
    sti();
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
    if (c == '\b'){
        if(term_buf_location > 0){
            delete();
            term_buf_location--;
        }
        return;
    }

    //get correct newline behavior
    if (c == '\n' || c == '\r')
        term_buf_location = 0;
    if (term_buf_location >= TERM_BUF_SIZE)
        return;

    putc(c);  
    term_buf_location++; 
}
