#include "tests.h"
#include "x86_desc.h"
#include "lib.h"

#define PASS 1
#define FAIL 0
#define TEST_DIV_BY_0 1
#define VIDEO 0xB8000

/* format these macros as you see fit */
#define TEST_HEADER \
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result) \
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure()
{
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}

#define PRINT_WHERE \
	printf("In %s at %s:%d\n", __FUNCTION__, __FILE__, __LINE__)

/* Checkpoint 1 tests */

/* IDT Test - Example
 *
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int test_idt()
{
	TEST_HEADER;
	// printf("\n\n\n\n\n\nn\n\n\n");
	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i)
	{
		PRINT_WHERE;
		if ((idt[i].offset_15_00 == NULL) &&
				(idt[i].offset_31_16 == NULL))
		{
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}
/* div0 Test - Example
 * Asserts that dividing by zero will return an exception.
 * Inputs: None
 * Outputs: None
 * Side Effects: Goes to while loop
 * Coverage: IDT Exception, div 0
 * Files: idt.c
 */
int test_div_by_zero()
{
	TEST_HEADER;
	int zero = 0;
	int one = 1;

	printf("I am about to divide by 0. ");
	printf("%d", one / zero);
	return 0;

}
/* Assertion Fail Test
 * Asserts that asserting will assert that the program goes into a while loop
 * Inputs: None
 * Outputs: None
 * Side Effects: Goes to while loop
 * Coverage: IDT Exception, assert
 * Files: idt.c
 */
int test_assertion_fail()
{
	asm volatile(
			"int $15");
	return 0;
}

/* Interrupt Test
 * Asserts that interrupts are handled
 * Inputs: None
 * Outputs: None
 * Side Effects: Goes to while loop
 * Coverage: IDT Exception, interrupt
 * Files: idt.c
 */
int test_sys_interrupt()
{
	asm volatile(
			"int $128"); // directly call system interrupt.
	return 0;

}

/* RTC Interrupt
 * Asserts that a RTC Interrupt will be caught.
 * Inputs: None
 * Outputs: None
 * Side Effects: Goes to while loop
 * Coverage: IDT Exception
 * Files: idt.c
 */
int test_rtc()
{
	TEST_HEADER;
	test_interrupts();

	// TODO Put RTC intterupt test here from rtc.h
	// waiting for RTC to be done
	return PASS;

}

/* Scancode keyboard test
 * Tests some keyboard scancodes
 * Inputs: None
 * Outputs: None
 * Side Effects: Goes to while loop
 * Coverage: IDT Exception
 * Files: idt.c
 */
int test_keyb_scancode()
{
	uint32_t i;
	TEST_HEADER;
	// TODO handle keypress
	// waiting on keyboards

	printf("Go ahead, press some keys :)\n");
	printf("This for loop goes for 5000m iterations. Have fun!");
	for (i = 0; i < 500000000; ++i);
	return PASS;

}

/* Paging struct test
 * Tests the paging structs
 * Inputs: None
 * Outputs: FAIL/PASS
 * Side Effects: None
 * Coverage: Paging structs
 * Files: paging.c
 */
int test_paging_struct_test()
{
	int test_status = FAIL;
	TEST_HEADER;
	// TODO check paging struct
	// waiting on paging
	return test_status;
}

/* Paging deref test
 * Tests the dereferencing of addresses with paging
 * Inputs: None
 * Outputs: PASS or crash
 * Side Effects: crashes if failure
 * Coverage: Paging structs
 * Files: paging.c
 */
int test_paging_struct_dref()
{
	TEST_HEADER;
	// TODO ensure paging is active
	int data, i;
	int *ptr;

	ptr = (int *)VIDEO;
	data = *ptr;
	for (i = 0; i < 5; i++)
	{
		ptr = (int *)data;
		data = *ptr;
	}
	return PASS;
}

int test_dereference_null()
{
    TEST_HEADER;
	int test_result = FAIL;
	printf("initializing variable x = 5\n");
	printf("initializing a pointer p = NULL\n");
    int *ptr;
    ptr = NULL;
    int x;
    x = *ptr;

	test_result = PASS;
	return test_result;
}

// add more tests here

/* Checkpoint 2 tests */
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */

/* Test suite entry point */
void launch_tests() {
	// TEST_OUTPUT("idt_test", idt_test());
	// // launch your tests here
	//
	clear();
	// TEST_OUTPUT("Test div by zero", test_div_by_zero());
	// TEST_OUTPUT("Test dereference null", test_dereference_null());
	// TEST_OUTPUT("Test the keyboard", test_keyb_scancode());
	// TEST_OUTPUT("Test RTC", test_rtc());
	TEST_OUTPUT("Test a sample system interrupt", test_sys_interrupt());

}
