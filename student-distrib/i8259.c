/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/*
 * i8259_init
 *   DESCRIPTION: Initialize the 8259 PIC
 *   INPUTS: none
 *   OUTPUTS: PIC initialization
 *   RETURN VALUE: none
 */
void i8259_init(void) {
    // Using https://wiki.osdev.org/8259_PIC as reference

    // Start initialization sequence
    outb(ICW1, MASTER_8259_PORT);
    outb(ICW1, SLAVE_8259_PORT);

    // Send high bits of vector number
    outb(ICW2_MASTER, MASTER_DATA_PORT);
    outb(ICW2_SLAVE, SLAVE_DATA_PORT);

    // Tell slave master exists, give slave address
    outb(ICW3_MASTER, MASTER_DATA_PORT);
    outb(ICW3_SLAVE, SLAVE_DATA_PORT);

    // Send normal/auto EOI
    outb(ICW4, MASTER_DATA_PORT);
    outb(ICW4, SLAVE_DATA_PORT);

    // Mask data sent to PIC
    outb(PORT_MASK, MASTER_DATA_PORT);
    outb(PORT_MASK, SLAVE_DATA_PORT);

    // Enable slave interrupts
    enable_irq(ICW3_SLAVE);
}

/*
 * enable_irq
 *   DESCRIPTION: Enable (unmask) the specified IRQ
 *   INPUTS: irq_num - IRQ number of desired device (0-15 inclusive)
 *   OUTPUTS: IRQ of desired device will be enabled
 *   RETURN VALUE: none
 */
void enable_irq(uint32_t irq_num) {
    // Using https://wiki.osdev.org/8259_PIC as reference

    // Ensure input parameter is within bounds
    if(irq_num < 0 || irq_num > MAX_DEVICES)
        return;

    uint16_t port;
    uint8_t value;

    if(irq_num < MAX_MASTER_DEVICES) {
        // Set port as master
        port = MASTER_DATA_PORT;
    } else {
        // Set port as slave, change irq_num to get correct location
        port = SLAVE_DATA_PORT;
        irq_num -= MAX_MASTER_DEVICES;
    }

    // Send desired command
    value = inb(port) & ~(1 << irq_num);
    outb(value, port);
}

/*
 * disable_irq
 *   DESCRIPTION: Disable (mask) the specified IRQ
 *   INPUTS: irq_num - IRQ number of desired device (0-15 inclusive)
 *   OUTPUTS: IRQ of desired device will be disabled
 *   RETURN VALUE: none
 */
void disable_irq(uint32_t irq_num) {
    // Using https://wiki.osdev.org/8259_PIC as reference

    // Ensure input parameter is within bounds
    if(irq_num < 0 || irq_num > MAX_DEVICES)
        return;

    uint16_t port;
    uint8_t value;

    if(irq_num < MAX_MASTER_DEVICES) {
        // Set port as master
        port = MASTER_DATA_PORT;
    } else {
        // Set port as slave, change irq_num to get correct location
        port = SLAVE_DATA_PORT;
        irq_num -= MAX_MASTER_DEVICES;
    }

    // Send desired command
    value = inb(port) | (1 << irq_num);
    outb(value, port);
}

/*
 * send_eoi
 *   DESCRIPTION: Send end-of-interrupt signal for the specified IRQ
 *   INPUTS: irq_num - IRQ number of desired device (0-15 inclusive)
 *   OUTPUTS: IRQ of desired device will recieve EOI command
 *   RETURN VALUE: none
 */
void send_eoi(uint32_t irq_num) {
    // Using https://wiki.osdev.org/8259_PIC as reference

    // Ensure input parameter is within bounds
    if(irq_num < 0 || irq_num > MAX_DEVICES)
        return;

    if(irq_num < MAX_MASTER_DEVICES) {
        // Send EOI to master
        outb(EOI | irq_num, MASTER_8259_PORT);
    } else {
        // Send EOI to slave (and master)
        irq_num -= MAX_MASTER_DEVICES;
        outb(EOI | irq_num, SLAVE_8259_PORT);
        outb(EOI | ICW3_SLAVE, MASTER_8259_PORT);
    }
}
