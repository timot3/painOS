#ifndef RTC_H
#define RTC_H

#include "lib.h"
#include "i8259.h"
#include "x86_desc.h"
#include "tests.h"

// From https://wiki.osdev.org/RTC
#define RTC_PORT 0x70
#define CMOS_PORT 0x71
#define RTC_A 0x8A
#define RTC_B 0x8B
#define RTC_C 0x0C

// IRQ number for RTC - Given in lectures/notes
#define RTC_IRQ 0x8

// Functions currently required for RTC stuff
void initialize_rtc();
void rtc_handler();
int setFrequency(int freq);
int rtc_open();
int rtc_close();
int rtc_read();
int rtc_write();

#endif /* RTC_H */
