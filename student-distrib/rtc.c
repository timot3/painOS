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
    // Using https://wiki.osdev.org/RTC as reference

    // Set frequency to 2HZ
    // int freq = 0x0F;
    //
    // cli();
    //
    // outb(RTC_A, RTC_PORT);
    // char prev = inb(CMOS_PORT);
    // outb(RTC_A, RTC_PORT);
    // outb((prev & 0xF0) | freq, CMOS_PORT);
    //
    // sti();
    //
    // return 0;

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
    // Frequency values based on https://courses.grainger.illinois.edu/ece391/sp2021/secure/references/ds12887.pdf table 3 (pg 19)
    // Convert desired frequency into bits for register A, return -1 if frequency is not valid
    int regAVals;
    switch(freq) {
        case 8192:
            regAVals = 0x03;
            break;
        case 4096:
            regAVals = 0x04;
            break;
        case 2048:
            regAVals = 0x05;
            break;
        case 1024:
            regAVals = 0x06;
            break;
        case 512:
            regAVals = 0x07;
            break;
        case 256:
            regAVals = 0x08;
            break;
        case 128:
            regAVals = 0x09;
            break;
        case 64:
            regAVals = 0x0A;
            break;
        case 32:
            regAVals = 0x0B;
            break;
        case 16:
            regAVals = 0x0C;
            break;
        case 8:
            regAVals = 0x0D;
            break;
        case 4:
            regAVals = 0x0E;
            break;
        case 2:
            regAVals = 0x0F;
            break;
        default:
            return -1;
    }

    cli();

    // Set frequency of RTC
    // Using https://wiki.osdev.org/RTC as reference
    outb(RTC_A, RTC_PORT);
    char prev = inb(CMOS_PORT);
    outb(RTC_A, RTC_PORT);
    outb((prev & 0xF0) | regAVals, CMOS_PORT);

    sti();

    return 0;
}
