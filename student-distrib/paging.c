#include "paging.h"

// Assembly functions from enable_paging.S
extern void edit_paging_flags(int page_dir_addr);
extern void tlb_flush();

// align with 4KB Chunks
page_dir_entry_t page_dir[PAGE_DIRECTORY_LENGTH] __attribute__((aligned(PAGE_DIRECTORY_LENGTH * 4)));
page_table_entry_t page_table[PAGE_TABLE_LENGTH] __attribute__((aligned(PAGE_TABLE_LENGTH * 4)));
page_table_entry_t vid_page_table[PAGE_TABLE_LENGTH] __attribute__((aligned(PAGE_TABLE_LENGTH * 4)));
/*
 * paging_init
 *   DESCRIPTION: Initializes the paging
 *   INPUTS: none
 *   OUTPUTS: None
 *   RETURN VALUE: None
 *   Side Effects:
 * * Changes Control Registers
 * * Creates Page Directory
 * * Creates Page Table(s)
 * * Implements paging for kernel space
 * * Implements paging for Video memory
 */
void paging_init()
{
  int i;

  for (i = 0; i < PAGE_DIRECTORY_LENGTH; i++)
  {
    page_dir[i].val = 0; // clear contents
    // set to rw
    page_dir[i].rw = 0;
    page_dir[i].us = 0;
  }

  for (i = 0; i < PAGE_TABLE_LENGTH; i++)
  {
    page_table[i].val = 0; // clear contents
    // set to rw
    page_table[i].rw = 0;
    page_table[i].us = 0;
    // set address
    page_table[i].aligned_address = i;
  }

  // connect PDT to page table
  page_dir[0].aligned_address = ((int) &page_table) >> ADDRESS_SHIFT;
  page_dir[0].present = 1;
  page_dir[0].cache_disable = 1;


  // connect 4-8 MB memory
  page_dir[1].aligned_address = KERNEL_LOCATION >> ADDRESS_SHIFT;
  page_dir[1].present = 1;
  page_dir[1].rw = 1;
  page_dir[1].size = 1; // 4 MB

  // connect video memory 32kb
  int j;
  for(j=0; j<8; j++){
    page_table[VID_MEM+j].aligned_address = 0xB8 + j;
    page_table[VID_MEM+j].present = 1;
    page_table[VID_MEM+j].cache_disable = 1;
  }

  edit_paging_flags((int)page_dir);
}

/*
 * get_paging_directory
 *   DESCRIPTION: Copies page_dir for testing
 *   INPUTS: page_dir_alt: new page dir to copy into
 *           len: length of page dir to copy
 *   OUTPUTS: copies data of page_dir
 *   RETURN VALUE: none
 */
extern void get_paging_directory(page_dir_entry_t *page_dir_alt, int len){
  int i;
  for(i = 0; i<len; i++)
   {
      page_dir_alt[i]=page_dir[i];
   }
}

/*
 * get_paging_table
 *   DESCRIPTION: Copies page_table for testing
*   INPUTS: page_dir_alt: new page dir to copy into
 *           len: length *   OUTPUTS: copies data of page_table
 *   RETURN VALUE: none
 */

extern void get_paging_table(page_table_entry_t *page_table_alt, int len) {
  int i;
  for(i = 0; i<len; i++)
   {
      page_table_alt[i]=page_table[i];
   }
}

/*
* map_page_pid
*   DESCRIPTION: map virtual address to physical address based on pid https://wiki.osdev.org/Paging#Enabling
*   INPUTS: pid
*   OUTPUTS: maps virtual address to physical address
*   RETURN VALUE: none
*/
void map_page_pid(int pid) {
  int phys_addr = KERNEL_PAGE + pid * TASK_SIZE;
  int page_idx = CORRECT_PAGE;
  page_dir[page_idx].present = 1;
  page_dir[page_idx].rw = 1;
  page_dir[page_idx].us = 1;
  page_dir[page_idx].size = 1;
  page_dir[page_idx].aligned_address = phys_addr >> 12;

  tlb_flush();
}

/*
* map_page_vid
*   DESCRIPTION: map virtual address for vid mem to physical address based on pid https://wiki.osdev.org/Paging#Enabling
*   INPUTS: virtual address
*   OUTPUTS: maps virtual address to physical address
*   SIDE EFFECTS: Flushes TLB
*   RETURN VALUE: none
*/
void map_page_vid(int virt_addr) {
  page_dir[virt_addr].present = 1;
  page_dir[virt_addr].rw = 1;
  page_dir[virt_addr].us = 1;
  // page_dir[page_idx].size = 1;
  page_dir[virt_addr].aligned_address =  ((int) &vid_page_table) >> 12;

  vid_page_table[0].present = 1;
  vid_page_table[0].rw = 1;
  vid_page_table[0].us = 1; // user space video mem
  vid_page_table[0].aligned_address = VID_MEM;

  tlb_flush();
}

