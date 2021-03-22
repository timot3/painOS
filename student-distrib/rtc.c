#include "rtc.h"
#include "lib.h"
#include "i8259.h"
#include "x86_desc.h"

/*
 * initialize_rtc
 *   DESCRIPTION: Initializes the RTC
 *   INPUTS: none
 *   OUTPUTS: makes RTC work
 *   RETURN VALUE: none
 */
void initialize_rtc() {
    // Using https://wiki.osdev.org/RTC as reference

    cli();
    // Disable NMI
    outb(RTC_B, RTC_PORT);
    // Read value of register B
    char prev = inb(CMOS_PORT);

    // Set index again (read resets index to register D)
    outb(RTC_B, RTC_PORT);
    // Write previous value read ORed with 0x40 -> turns on bit 6 of register B
    outb(prev | 0x40, CMOS_PORT);
    sti();

    // Enable interrupt requests for RTC on PIC
    enable_irq(RTC_IRQ);
}

/*
 * rtc_handler
 *   DESCRIPTION: called every time RTC sends interrupt command
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
void rtc_handler() {
    // Clear register C otherwise interrupt won't happen again
    // - currently don't care about its value
    outb(RTC_C, RTC_PORT);
    inb(CMOS_PORT);

    // Comment out test_interrupts to make screen not weird
    //test_interrupts();
    sti();

    // Send EOI so device knows we're done
    send_eoi(RTC_IRQ);
}
