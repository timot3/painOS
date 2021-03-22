#ifndef TESTS_H
#define TESTS_H

// test declarations
int test_idt();
int test_div_by_zero();
int test_assertion_fail();
int test_sys_interrupt();
int test_rtc();
int test_keyb_scancode();
int test_paging_struct_test();
int test_paging_struct_dref();
int test_dereference_null();

void launch_tests();

#endif /* TESTS_H */
