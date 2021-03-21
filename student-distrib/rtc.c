#include "rtc.h"
#include "lib.h"

void initialize_rtc() {
    // Using https://wiki.osdev.org/RTC as reference
    cli();
    outb(RTC_B, RTC_PORT);
    char prev = inb(CMOS_PORT);
    outb(RTC_B, RTC_PORT);
    outb(prev | 0x40, CMOS_PORT);
    enable_irq(RTC_IRQ);
}
