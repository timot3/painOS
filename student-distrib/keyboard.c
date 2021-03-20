#include "keyboard.h"
#include "lib.h"

char = scan
//0's in order stand in for nothing, escape, enter, left shift
char scan_code_1[128] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 'a'
    's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
    '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n' 'm',
    }

void keyboard_init(){
    enable_irq(KB_IRQ)
}

void keyboard_handler(){
    byte = inb(KB_PORT);
}
