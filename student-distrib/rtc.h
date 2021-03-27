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

// default RTC frequency is 2Hz
#define RTC_DEFAULT_FREQUENCY 2

// Functions currently required for RTC stuff
void initialize_rtc();
void rtc_handler();
uint8_t set_frequency(uint16_t freq);
int rtc_open();
uint8_t rtc_close();
uint8_t rtc_read();
int rtc_write(void *buffer, uint32_t size);

#endif /* RTC_H */
