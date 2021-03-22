#include "rtc.h"
#include "lib.h"
#include "i8259.h"
#include "x86_desc.h"

void initialize_rtc() {
    // Using https://wiki.osdev.org/RTC as reference

    cli();
    outb(RTC_B, RTC_PORT);
    char prev = inb(CMOS_PORT);

    outb(RTC_B, RTC_PORT);
    outb(prev | 0x40, CMOS_PORT);
    sti();
    enable_irq(RTC_IRQ);
}

void rtc_handler() {
    //cli();
    outb(0x0C, RTC_PORT);
    inb(CMOS_PORT);
    // printf("rtc_handler\n");
    // test_interrupts();
    sti();
    send_eoi(RTC_IRQ);

}
