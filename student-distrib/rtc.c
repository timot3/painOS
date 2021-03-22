#include "rtc.h"
#include "lib.h"
#include "i8259.h"
#include "x86_desc.h"

void initialize_rtc() {
    // Using https://wiki.osdev.org/RTC as reference


    cli();        // important that no interrupts happen (perform a CLI)
    outb(0x8A, RTC_PORT);    // select Status Register A, and disable NMI (by setting the 0x80 bit)
    outb(0x20, CMOS_PORT);    // write to CMOS/RTC RAM
    sti();        // (perform an STI) and reenable NMI if you wish

    cli();
    outb(RTC_B, RTC_PORT);
    char prev = inb(CMOS_PORT);

    outb(RTC_B, RTC_PORT);
    outb(prev | 0x40, CMOS_PORT);
    sti();
    enable_irq(RTC_IRQ);
    printf("after irq.\n");


    // Changes frequency
    int rate = 1;
    rate &= 0x0F;			// rate must be above 2 and not over 15
    cli();
    outb(0x8A, 0x70);		// set index to register A, disable NMI
    prev=inb(0x71);	// get initial value of register A
    outb(0x8A, 0x70);		// reset index to A
    outb(0x2C, 0x71); //write only our rate to A. Note, rate is the bottom 4 bits.
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
