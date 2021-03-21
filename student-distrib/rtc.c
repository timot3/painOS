#include "rtc.h"
#include "lib.h"

void initialize_rtc() {
    // Using https://wiki.osdev.org/RTC as reference

    cli();
    outb(RTC_B, RTC_PORT);        // Disable NMI
    char prev = inb(CMOS_PORT);   // Read current value
    outb(RTC_B, RTC_PORT);        // Set index again (read resets index to register D)
    outb(prev | 0x40, CMOS_PORT); // Turn off bit 6 of register B
    enable_irq(RTC_IRQ);
}
