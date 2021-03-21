#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KB_PORT 0x60
#define KB_IRQ 1


void keyboard_init();
void keyboard_handler();

#endif /* KEYBOARD_H */
