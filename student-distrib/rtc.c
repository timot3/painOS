#include "rtc.h"

// Flag for interrupt
volatile int interruptFlags[MAX_TERMINALS] = {};

/*
 * initialize_rtc
 *   DESCRIPTION: Initializes the RTC
 *   INPUTS: none
 *   OUTPUTS: makes RTC work
 *   RETURN VALUE: none
 */
void initialize_rtc() {
    // Using https://wiki.osdev.org/RTC as reference

    // cli();
    // Disable NMI
    outb(RTC_B, RTC_PORT);
    // Read value of register B
    char prev = inb(CMOS_PORT);

    // Set index again (read resets index to register D)
    outb(RTC_B, RTC_PORT);
    // Write previous value read ORed with 0x40 -> turns on bit 6 of register B
    outb(prev | 0x40, CMOS_PORT);

    // Enable interrupt requests for RTC on PIC
    enable_irq(RTC_IRQ);
}

/*
 * rtc_handler
 *   DESCRIPTION: Called every time RTC sends interrupt command
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
void rtc_handler() {
    // cli();

    // Set interrupt flag
    interruptFlags[current_terminal] = 1;

    #ifdef RTC_TEST
        test_interrupts();
    #endif

    // Clear register C otherwise interrupt won't happen again
    // - currently don't care about its value
    outb(RTC_C, RTC_PORT);
    inb(CMOS_PORT);

    // Send EOI so device knows we're done
    send_eoi(RTC_IRQ);
}

/*
 * rtc_open
 *   DESCRIPTION: Sets RTC frequency to 2Hz
 *   INPUTS: *filename - never used, docs said we needed it
 *   OUTPUTS: none
 *   RETURN VALUE: 0 on success, -1 on failure (based on discussion slides)
 */
int32_t rtc_open(const uint8_t *filename) {
    // Use helper function to set frequency to 2Hz
    return set_frequency(RTC_DEFAULT_FREQUENCY);
}

/*
 * rtc_close
 *   DESCRIPTION: Currently does nothing since we aren't virtualizing the RTC
 *   INPUTS: fd - never used, docs said we needed it
 *   OUTPUTS: none
 *   RETURN VALUE: Always returns 0 (based on discussion slides)
 */
int32_t rtc_close(int32_t fd) {
    return 0;
}

/*
 * rtc_read
 *   DESCRIPTION: Returns once new RTC interrupt has occured
 *   INPUTS: fd - never used, docs said we needed it
 *            *buf - never used, docs said we needed it
 *            nbytes - never used, docs said we needed it
 *   OUTPUTS: none
 *   RETURN VALUE: Always returns 0 (based on discussion slides)
 */
int32_t rtc_read(int32_t fd, void *buf, int32_t nbytes) {
    interruptFlags[current_terminal] = 0;

    // Spin until new interrupt occurs
   sti();
   volatile uint32_t i;
//    for (i = 0; i < 0xFFFFFF; i++);
    while(interruptFlags[current_terminal] == 0);
    cli(); 
    return 0;
}

/*
 * rtc_write
 *   DESCRIPTION: Sets RTC frequency based on input value
 *   INPUTS: fd - never used, docs said we needed it
 *           *buf - buffer holding new frequency (not sure why it needs to
 *                     be a buffer, but that's what the discussion said to use)
 *           nbytes - size of buffer
 *   OUTPUTS: none
 *   RETURN VALUE: -1 in invalid frequency input, 0 otherwise
 */
int32_t rtc_write(int32_t fd, const void *buf, int32_t nbytes) {
    // Change frequency using helper function
    return set_frequency(*(uint16_t*)buf);
}

/*
 * set_frequency
 *   DESCRIPTION: Helper function to seq frequency of RTC based on input int
 *   INPUTS: freq - input frequency in Hz
 *   OUTPUTS: none
 *   RETURN VALUE: -1 on invalid input (not in range, not power of 2), 0 otherwise
 */
int32_t set_frequency(uint16_t freq) {
    // Ensure value is within bounds (limited by kernel to 1024, can go to 8192) and is power of 2
    if(freq < 1 || freq > 1024 || (freq & (freq - 1)) != 0)
        return -1;

    // Frequency values based on https://courses.grainger.illinois.edu/ece391/sp2021/secure/references/ds12887.pdf table 3 (pg 19)
    // Convert desired frequency into bits for register A
    int regAVals = 0x00;

    while(freq > 1) {
        freq /= 2;
        regAVals++;
    }

    // 0xF is the maximum value (value set when frequency = 2Hz) -> needed since more bits are on when frequency is lower
    regAVals = 0xF - regAVals + 1;

    // Set frequency of RTC
    // Using https://wiki.osdev.org/RTC as reference
    // cli();
    outb(RTC_A, RTC_PORT);
    char prev = inb(CMOS_PORT);
    outb(RTC_A, RTC_PORT);
    outb((prev & 0xF0) | regAVals, CMOS_PORT);

    return 0;
}
