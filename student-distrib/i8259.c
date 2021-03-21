/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask = 0xFF; /* IRQs 0-7  */
uint8_t slave_mask = 0xFF;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {
    // Using https://wiki.osdev.org/8259_PIC as reference

    // Start initialization sequence
    outb(ICW1, MASTER_8259_PORT);
    outb(ICW1, SLAVE_8259_PORT);

    // Send high bits of vector number
    outb(ICW2_MASTER, MASTER_8259_PORT);
    outb(ICW2_SLAVE, SLAVE_8259_PORT);

    // Tell slave master exists, give slave address
    outb(ICW3_MASTER, MASTER_8259_PORT);
    outb(ICW3_SLAVE, SLAVE_8259_PORT);

    // Send normal/auto EOI
    outb(ICW4, MASTER_8259_PORT);
    outb(ICW4, SLAVE_8259_PORT);

    // Enable slave interrupts
    enable_irq(SLAVE_8259_PORT);
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    // Using https://wiki.osdev.org/8259_PIC as reference

    uint16_t port;
    uint8_t value;
 
    if(irq_num < 8) {
        port = MASTER_8259_PORT;
    } else {
        port = SLAVE_8259_PORT;
        irq_num -= 8;
    }
    value = inb(port) | (1 << irq_num);
    outb(value, port); 
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    // Using https://wiki.osdev.org/8259_PIC as reference

    uint16_t port;
    uint8_t value;
 
    if(irq_num < 8) {
        port = MASTER_8259_PORT;
    } else {
        port = SLAVE_8259_PORT;
        irq_num -= 8;
    }
    value = inb(port) & ~(1 << irq_num);
    outb(value, port); 
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
    // Using https://wiki.osdev.org/8259_PIC as reference

    if(irq_num < 8) {
        // Send master EOI
        outb(EOI | irq_num, MASTER_8259_PORT);
    } else {
        // Send slave (and master) EOI
        irq_num -= 8;
        outb(EOI | irq_num, SLAVE_8259_PORT);
        outb(EOI | ICW3_SLAVE, MASTER_8259_PORT);
    }
}
