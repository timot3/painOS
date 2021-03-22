#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "paging.h"

#define PASS 1
#define FAIL 0
#define TEST_DIV_BY_0 1
#define VIDEO 0xB8000


/* format these macros as you see fit */
#define TEST_HEADER \
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result) \
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");
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
	asm volatile("int $15");
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
	asm volatile("int $80");
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
	// test_interrupts();

	printf("This test has been moved to a compiler macro in tests.h.\n");
	printf(" Comment it out to not test.\n");

	return PASS;

}

/* Paging struct test
 * Tests the paging structs for members
 * Inputs: None
 * Outputs: PASS 
 * Side Effects: Crashes on failure
 * Coverage: Paging structs
 * Files: paging.c
 */
int test_paging_struct()
{
	int test_status = FAIL;
	TEST_HEADER;
	int tmp, i;
	page_dir_entry_t page_dir[PAGE_DIRECTORY_LENGTH];
	get_paging_directory(page_dir, PAGE_DIRECTORY_LENGTH);

	page_table_entry_t page_table[PAGE_TABLE_LENGTH];
	get_paging_table(page_table, PAGE_TABLE_LENGTH);
	
	
	// These will error out if a member doesn't exist
	for (i = 0; i <PAGE_DIRECTORY_LENGTH; i++){
		tmp = page_dir[i].val;
		tmp = page_dir[i].present;
		tmp = page_dir[i].rw;
		tmp = page_dir[i].us;
		tmp = page_dir[i].write_through;
		tmp = page_dir[i].cache_disable;
		tmp = page_dir[i].accessed;
		tmp = page_dir[i].zero;
		tmp = page_dir[i].size;
		tmp = page_dir[i].ignored;
		tmp = page_dir[i].available;
		tmp = page_dir[i].aligned_address;
	}
	

	// check the members for table_entry
	for (i = 0; i <PAGE_DIRECTORY_LENGTH; i++){
		tmp = page_table[i].val;
		tmp = page_table[i].present;
		tmp = page_table[i].rw;
		tmp = page_table[i].us;
		tmp = page_table[i].write_through;
		tmp = page_table[i].cache_disable;
		tmp = page_table[i].accessed;
		tmp = page_table[i].dirty;
		tmp = page_table[i].zero;
		tmp = page_table[i].global;
		tmp = page_table[i].available;
		tmp = page_table[i].aligned_address;
	}

	return PASS;
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
	int data, i;
	int *ptr;

	ptr = (int *)VIDEO;
	*ptr = 42;
	for (i = 0; i < 5; i++)
	{
		data = *ptr;
		ptr = &data;
		if (*ptr != data || &data != ptr){
			return FAIL;
		}
	}
	return PASS;
}

int test_dereference_null()
{
    int *ptr;
    int x = 5;
	int test_status = FAIL;


    TEST_HEADER;
	printf("initializing variable x = 5\n");
	printf("initializing a pointer p = NULL\n");
    ptr = NULL;
	printf("setting x = *ptr\n");
    x = *ptr;

	printf("x = %d\n", x);

	test_status = PASS;
	return test_status;
}

// add more tests here

/* Checkpoint 2 tests */
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */

/* Test suite entry point */
void launch_tests() {
	// TEST_OUTPUT("idt_test", idt_test());
	// launch your tests here
	//clear();
	// TEST_OUTPUT("Paging Structs Members+Values", test_paging_struct());
	// TEST_OUTPUT("Paging Dereferencing", test_paging_struct_dref());
	// TEST_OUTPUT("Test div by zero", test_div_by_zero());
	// TEST_OUTPUT("Test dereference null", test_dereference_null());
	// TEST_OUTPUT("Test RTC", test_rtc());
	// TEST_OUTPUT("Test Assertion Fail", test_assertion_fail());
	// TEST_OUTPUT("Test System Interrupt", test_sys_interrupt());
}
