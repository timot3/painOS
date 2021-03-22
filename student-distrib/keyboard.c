#include "keyboard.h"
#include "lib.h"
#include "i8259.h"

//0's in order stand in for nothing, escape, enter, left shift, right shift, left alt
//https://wiki.osdev.org/PS/2_Keyboard
char scan_code_1[256] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 'a',
    's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
    '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm',
    ',', '.', '/', 0, '*', 0, ' '};

void keyboard_init(){
    enable_irq(KB_IRQ);
}

void keyboard_handler(){
    cli();
    char byte = inb(KB_PORT);
    char character = scan_code_1[byte];
    putc(character);
    send_eoi(KB_IRQ);
    sti();
}
