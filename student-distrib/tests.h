#ifndef TESTS_H
#define TESTS_H

// test declarations
int idt_test();
int div_by_zero();
int assertion_fail_test();
int interrupt_test();
int rtc_test();
int keyb_scancode();
int paging_struct_test();
int paging_struct_dref();
int dereference_null();

void launch_tests();

#endif /* TESTS_H */
