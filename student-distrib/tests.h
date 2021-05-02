#ifndef TESTS_H
#define TESTS_H

// test declarations

// Checkpoint 1 tests
int test_idt();
int test_div_by_zero();
int test_sys_interrupt();
int test_rtc();
int test_keyb_scancode();
int test_paging_struct_test();
int test_paging_struct_dref();
int test_dereference_null();
// #define RTC_TEST 1

// Checkpoint 2 tests
int test_rtc_freq();
int test_rtc_write();
int test_terminal();
int test_read_directory();
int test_file_open();
int test_open_bad_file();
int test_read_dentry_by_index();
int test_read_dentry_by_name();
int test_file_read();
int test_read_large();
int test_file_read_exe();


// checkpoint 3 tests
int test_bad_input_read();
int test_bad_input_write();
int test_bad_input_open();
int test_bad_input_close();

void launch_tests();

#endif /* TESTS_H */
