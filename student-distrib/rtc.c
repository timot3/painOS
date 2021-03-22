#include "rtc.h"
#include "lib.h"
#include "i8259.h"
#include "x86_desc.h"

void initialize_rtc() {
    // Using https://wiki.osdev.org/RTC as reference


    // cli();        // important that no interrupts happen (perform a CLI)
    // outb(0x8A, RTC_PORT);    // select Status Register A, and disable NMI (by setting the 0x80 bit)
    // char tmp = inb(CMOS_PORT);
    // outb(RTC_B, CMOS_PORT);        // Disable NMI

    // outb(0x20 | tmp, CMOS_PORT);    // write to CMOS/RTC RAM
    // sti();        // (perform an STI) and reenable NMI if you wish


    // enable irq 8 -- osdev
    cli();
    outb(RTC_B, RTC_PORT);        // Disable NMI
    char prev = inb(CMOS_PORT);   // Read current value
    outb(RTC_B, RTC_PORT);        // Set index again (read resets index to register D)
    outb(prev | 0x40, CMOS_PORT); // Turn off bit 6 of register B
    printf("before irq -- prev = %d\n", (int)prev);
    sti();
    //enable_irq(RTC_IRQ);
    printf("after irq.\n");

    // make rtc handler that dumps (write register c to cmd port, take in whatever at data port) the value at register C
    // Changes frequency
    int rate = 1;
    rate &= 0x0F;			// rate must be above 2 and not over 15
    cli();
    outb(RTC_A, RTC_PORT);		// set index to register A, disable NMI
    prev=inb(CMOS_PORT);	// get initial value of register A
    outb(RTC_A, RTC_PORT);		// reset index to A
    outb((prev & 0xF0) | rate, 0x71); //write only our rate to A. Note, rate is the bottom 4 bits.
    enable_irq(RTC_IRQ);
}

void runThing() {
    // if(idt[0x28].present == 0) {
    //     printf("PAIN\n");
    //     return;
    // }
    send_eoi(RTC_IRQ);
    test_interrupts();


    outb(RTC_C, RTC_PORT);	// select register C
    inb(CMOS_PORT);		// just throw away contents

}
