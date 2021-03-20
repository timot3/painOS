#ifndef IDT_H
#define IDT_H

void idt_error();

void initialize_idt();

#define NUM_SYSTEM_INTERRUPTS 32

#endif /* IDT_H */
