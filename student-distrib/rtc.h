#ifndef RTC_H
#define RTC_H

#define RTC_PORT 0x70
#define CMOS_PORT 0x71

#define RTC_A 0x8A
#define RTC_B 0x8B
#define RTC_C 0x8C

#define RTC_IRQ 0x8

void initialize_rtc();

#endif /* RTC_H */
