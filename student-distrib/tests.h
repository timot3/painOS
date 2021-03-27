#ifndef TESTS_H
#define TESTS_H

// test declarations
int test_idt();
int test_div_by_zero();
int test_sys_interrupt();
int test_rtc();
int test_keyb_scancode();
int test_paging_struct_test();
int test_paging_struct_dref();
int test_dereference_null();
int test_rtc_freq();
int test_rtc_write();

void launch_tests();

// #define RTC_TEST 1


#endif /* TESTS_H */
