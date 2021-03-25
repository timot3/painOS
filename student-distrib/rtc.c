#include "rtc.h"

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
    cli();

    #ifdef RTC_TEST
        test_interrupts();
    #endif

    // Clear register C otherwise interrupt won't happen again
    // - currently don't care about its value
    outb(RTC_C, RTC_PORT);
    inb(CMOS_PORT);
    sti();

    // Send EOI so device knows we're done
    send_eoi(RTC_IRQ);
}

/*
 * rtc_open
 *   DESCRIPTION: Sets RTC frequency to 2HZ
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: Always returns 0 (based on discussion slides)
 */
int rtc_open() {
    // Use helper function to set frequency to 2HZ
    return setFrequency(0x02);
}

/*
 * rtc_close
 *   DESCRIPTION:
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
int rtc_close() {
    return 0;
}

/*
 * rtc_read
 *   DESCRIPTION:
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
int rtc_read() {
    return 0;
}

/*
 * rtc_write
 *   DESCRIPTION:
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
int rtc_write(int freq) {
    // Change frequency
    return setFrequency(freq);
}

/*
 * setFrequency
 *   DESCRIPTION:
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
int setFrequency(int freq) {
    // Ensure value is within bounds and is power of 2
    if(freq < 1 || freq > 8192 || (freq & (freq - 1)) != 0)
        return -1;

    // Frequency values based on https://courses.grainger.illinois.edu/ece391/sp2021/secure/references/ds12887.pdf table 3 (pg 19)
    // Convert desired frequency into bits for register A
    int regAVals;

    while(freq > 1) {
        freq /= 2;
        regAVals++;
    }

    regAVals = 0xF - regAVals + 1;

    // Set frequency of RTC
    // Using https://wiki.osdev.org/RTC as reference
    cli();
    outb(RTC_A, RTC_PORT);
    char prev = inb(CMOS_PORT);
    outb(RTC_A, RTC_PORT);
    outb((prev & 0xF0) | regAVals, CMOS_PORT);

    sti();

    return 0;
}
