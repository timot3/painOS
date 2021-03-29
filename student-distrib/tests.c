#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "paging.h"
#include "rtc.h"
#include "filesys.h"

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

// add more tests here

/* Checkpoint 2 tests */

/* test_rtc_freq
 * Tests the RTC read and write functions
 * Inputs: None
 * Outputs: 1's printed to console at various frequencies
 * Side Effects: crashes if failure
 * Coverage: RTC frequency adjustment
 * Files: rtc.c
 */
int test_rtc_freq() {
	int i, j;
	TEST_HEADER;

	// Loop through all valid frequency values
	for(i = 2; i < 1025; i *= 2) {
		// if failed to set rtc frequency, set to false
		if (rtc_write(0, &i, sizeof(int)) == -1) {
			return FAIL;
		}

		// Print 10 1's per frequency
		for(j = 0; j < 10; j++) {
			rtc_read(0, 0, 0);
			printf("1");
		}

		printf("\n");
	}

	return PASS;
}

/* test_rtc_write
 * Tests the RTC open can set default value
 * Inputs: None
 * Outputs: None
 * Side Effects: None
 * Coverage: RTC frequency adjustment
 * Files: rtc.c
 */
int test_rtc_write() {
	TEST_HEADER;
	// If we can't set default rtc frequency, return 0.
	if (rtc_open(0) != 0) {
		return FAIL;
	}
	return PASS;
}

int read_directory() {
	TEST_HEADER;
	clear();
	int ret = dir_read(0, 0, 0) + 1;
	dir_close(0);

	return ret;
}

int test_file_open(){
	int32_t ret;
	uint8_t fname[32] = "ls\0";
	ret = file_open(fname);
	if (ret == -1){
		return FAIL;
	}
	return PASS;
}

int test_open_bad_file(){
	int32_t ret;
	uint8_t fname[32] = "thisisnotafile\0";
	ret = file_open(fname);
	if (ret == 0){
		return FAIL;
	}
	return PASS;
}

int test_read_dentry_by_index(){
	dentry_t test_dentry;
	uint32_t i, ret;
	for (i = 0; i<18; i++){
		ret = read_dentry_by_index(i, &test_dentry);
		if (i < 17 && ret == -1){

			return FAIL;
		}

		if (i == 17 && ret != -1){

			return FAIL;
		}

		if (i==0 && test_dentry.fname[0] != '.'){

			return FAIL;
		}

		if (i==6 && test_dentry.fname[0] != 'f'){

			return FAIL;
		}

		if (i==12 && test_dentry.fname[0] != 'l'){

			return FAIL;
		}

		if (i==16 && test_dentry.fname[0] != 'h'){

			return FAIL;
		}

	}
	return PASS;
}


int test_read_dentry_by_name(){
	dentry_t test_dentry;
	const uint8_t dot[MAX_CHAR] = ".\0";
	const uint8_t  fish[MAX_CHAR] = "fish\0";
	const uint8_t  ls[MAX_CHAR] = "ls\0";
	const uint8_t  hello[MAX_CHAR] = "hello\0";
	const uint8_t  garbage[MAX_CHAR] = "thisisnotafile\0";
	uint32_t ret;

	ret = read_dentry_by_name(dot, &test_dentry);
	if (ret == -1){

		return FAIL;
	}

	ret = read_dentry_by_name(fish, &test_dentry);
	if (ret == -1)
	{

		return FAIL;
	}

	ret = read_dentry_by_name(ls, &test_dentry);
	if (ret == -1){

		return FAIL;
	}

	ret = read_dentry_by_name(hello, &test_dentry);
	if (ret == -1)
	{

		return FAIL;
	}

	ret = read_dentry_by_name(garbage, &test_dentry);
	if (ret != -1)
	{

		return FAIL;
	}

	return PASS;
}

int test_file_read() {
	uint32_t ret;
	uint8_t buf_a[1], buf_b[1];

	// we don't know the actual first character, so we do this
	buf_a[0] = (uint8_t)'c';

	uint8_t buf_a2[1];
	buf_a2[0] = (uint8_t)'c';
	uint8_t fish[MAX_CHAR] = "frame0.txt\0";
	file_open(fish);
	ret = file_read(0, buf_a, 1);

	if (ret != 1){

		return FAIL;
	}
	if (buf_a[0] == 'c'){
		return FAIL;
	}

	ret = file_read(0, buf_a2, 1);
	if (ret != 1){
		return FAIL;
	}
	if (buf_a2[0] == 'c'){
		return FAIL;
	}

	// of course we want to ensure we're actually reading
	// properly, so we perform this too
	file_close(0);
	file_open(fish);
	ret = file_read(0, buf_b, 1);
	if (ret != 1){
		return FAIL;
	}
	if (buf_a[0] != buf_b[0]){
		return FAIL;
	}

	return PASS;
}

int test_read_data(){
	dentry_t test_dentry;

	uint8_t buf[10], test_name[11] = "frame0.txt\0";

	uint32_t inode_index;
	uint8_t i;

	read_dentry_by_name(test_name, &test_dentry);
	inode_index = test_dentry.inode;
	printf("INDEX: %d", inode_index);
	read_data(inode_index, 0, buf, 10);

	for(i = 0; i < 10; i++)  {
		putc((char)buf[i]);
	}

	putc('\n');

	return PASS;
}
int test_file_read_exe() {
	uint32_t ret, i, len=50;
	uint8_t buf_a[len];

	uint8_t frame[MAX_CHAR] = "sigtext\0";
	file_open(frame);
	ret = file_read(0, buf_a, len);
	for (i=0; i<len; i++){
		putc(buf_a[i]);
	}

	if ((int)buf_a[0] != 127){ // 0x7f
		printf("Failed test 1");
		return FAIL;
	}
	if ((int)buf_a[1] != 69){ // x45
		return FAIL;
	}
	if ((int)buf_a[2] != 127){ // 0x76
		return FAIL;
	}
	if ((int)buf_a[3] != 70){ // 0x4c
		return FAIL;
	}

	return PASS;
}


/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */

/* Test suite entry point */
void launch_tests() {
	// launch your tests here
	// CHECKPOINT 1
	// clear();
	// TEST_OUTPUT("Test idt", test_idt());
	// TEST_OUTPUT("Test div by zero", test_div_by_zero());
	// TEST_OUTPUT("Test RTC", test_rtc());
	// TEST_OUTPUT("Paging Structs Members+Values", test_paging_struct());
	// TEST_OUTPUT("Paging Dereferencing", test_paging_struct_dref());
	// TEST_OUTPUT("Test dereference null", test_dereference_null());
	// TEST_OUTPUT("Test dereference null", test_dereference_null());
	// TEST_OUTPUT("Test System Interrupt", test_sys_interrupt());


	// CHECKPOINT 2
	clear();
	// TEST_OUTPUT("Test rtc frequency adjustment", test_rtc_freq());
	// TEST_OUTPUT("Test rtc default frequency", test_rtc_write());
	// TEST_OUTPUT("Test test_file_read_exe", test_file_read_exe()); //bad
	// TEST_OUTPUT("Test test_read_data", test_read_data()); //bad
	// TEST_OUTPUT("Test test_file_read", test_file_read()); //bad
	// TEST_OUTPUT("Test test_read_dentry_by_name", test_read_dentry_by_name()); //works
	// TEST_OUTPUT("Test test_read_dentry_by_index", test_read_dentry_by_index()); //works
	// TEST_OUTPUT("Test test_open_bad_file", test_open_bad_file()); //works
	// TEST_OUTPUT("Test test_file_open", test_file_open()); //works
	// TEST_OUTPUT("Test read_directory", read_directory()); //works
}
