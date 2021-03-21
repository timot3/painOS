#include "rtc.h"
#include "lib.h"
<<<<<<< HEAD
#include "i8259.h"
=======
>>>>>>> origin/master

void initialize_rtc() {
    // Using https://wiki.osdev.org/RTC as reference

    cli();
    outb(RTC_B, RTC_PORT);        // Disable NMI
    char prev = inb(CMOS_PORT);   // Read current value
    outb(RTC_B, RTC_PORT);        // Set index again (read resets index to register D)
    outb(prev | 0x40, CMOS_PORT); // Turn off bit 6 of register B
    enable_irq(RTC_IRQ);
<<<<<<< HEAD

    // Changes frequency
    // int rate = 1;
    // rate &= 0x0F;			// rate must be above 2 and not over 15
    // cli();
    // outb(0x8A, 0x70);		// set index to register A, disable NMI
    // prev=inb(0x71);	// get initial value of register A
    // outb(0x8A, 0x70);		// reset index to A
    // outb((prev & 0xF0) | rate, 0x71); //write only our rate to A. Note, rate is the bottom 4 bits.
    // enable_irq(RTC_IRQ);
=======
>>>>>>> origin/master
}
