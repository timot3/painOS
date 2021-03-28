#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "paging.h"
#include "terminal.h"

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
	int i;
	int result = PASS;
	// check that the first 10 entries of the idt exist and aren't 0
	for (i = 0; i < 10; ++i)
	{
		// PRINT_WHERE;
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
	// get rid of compiler warnings
	int zero = 0;
	int one = 1;

	printf("I am about to divide by 0. ");
	// div by zero exception
	printf("%d", one / zero);
	return FAIL;

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
	asm volatile("int $128"); // directly call system interrupt
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
	printf("Uncomment it to test RTC.\n");

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
	// test that we can access memory we should be able to access
	ptr = (int *)VIDEO;
	*ptr = 42;
	// repeat 5 times
	for (i = 0; i < 5; i++)
	{
		data = *ptr;
		ptr = &data;
		// if access failed, then test failed
		if (*ptr != data || &data != ptr){
			return FAIL;
		}
	}
	return PASS;
}

/* dereference null test
 * Tests the dereferencing of null ptr
 * Inputs: None
 * Outputs: what is currently happening in the code
 * Side Effects: crashes if failure
 * Coverage: Paging structs
 * Files: paging.c
 */
int test_dereference_null()
{
	// init variables
    int *ptr;
    int x = 5;
	int test_status = FAIL;


    TEST_HEADER;
	printf("initializing variable x = 5\n");
	printf("initializing a pointer p = NULL\n");
    ptr = NULL;
	printf("setting x = *ptr\n");
	// should cause page fault
    x = *ptr;

	// should never reach here
	printf("x = %d\n", x);

	test_status = PASS;
	return test_status;
}

int test_terminal(){
	unsigned char test_buf[128];
	while(1){
		terminal_read(0, test_buf, 128);
		terminal_write(0, test_buf, 128);
	}
}

// add more tests here

/* Checkpoint 2 tests */
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */

/* Test suite entry point */
void launch_tests() {
	// launch your tests here
	// clear();
	// TEST_OUTPUT("Test idt", test_idt());
	// TEST_OUTPUT("Test div by zero", test_div_by_zero());
	// TEST_OUTPUT("Test RTC", test_rtc());
	// TEST_OUTPUT("Paging Structs Members+Values", test_paging_struct());
	// TEST_OUTPUT("Paging Dereferencing", test_paging_struct_dref());
	// TEST_OUTPUT("Test dereference null", test_dereference_null());
	// TEST_OUTPUT("Test System Interrupt", test_sys_interrupt());
	TEST_OUTPUT("Test Terminal", test_terminal());
}
