#include "paging.h"

page_dir_entry_t page_dir[PAGE_DIRECTORY_LENGTH] __attribute__((aligned(PAGE_DIRECTORY_LENGTH * 4)));
page_table_entry_t page_table[PAGE_TABLE_LENGTH] __attribute__((aligned(PAGE_TABLE_LENGTH * 4)));

void paging_init()
{
  int i;
  page_dir_entry_t tmp_dir;
  for (i = 0; i < PAGE_DIRECTORY_LENGTH; i++)
  {
    memset(page_dir[i], 0, sizeof(uint32_t)); // clear contents
    // set to rw
    page_dir[i].rw = 1;
  }

  for (i = 0; i < PAGE_TABLE_LENGTH; i++)
  {
    memset(page_table[i], 0, sizeof(uint32_t)); // clear contents
    // set to rw
    page_table[i].rw = 1;
    // set address
    page_table[i].address = i;
  }

  // connect PDT to page table
  tmp_dir = page_dir[0];
  tmp_dir.aligned_address = ((uint32_t)(page_table >> ADDRESS_SHIFT)
  tmp_dir.present = 1;
  page_table[0].present = 1;
  // rw should probably be 0
  // set us?
  // set write_through?

  // set first page dir to have proper contents
  page_dir[0].val = (uint32_t) tmp_dir.val;


  // connect 4-8 MB memory
  tmp_dir = page_dir[1];
  tmp_dir.aligned_address = KERNEL_LOCATION << ADDRESS_SHIFT;
  tmp_dir.present = 1;
  tmp_dir.rw = 1;
  tmp_dir.size = 1; // 4 MB
  page_dir[1].val = (uint32_t) tmp_dir.val;

  asm volatile("						        ;
      movl	$page_directory, %%eax  ;
      movl	%%eax, %%cr3			      ; 
      movl	%%cr4, %%eax			      ;
      orl		$0x10, %%eax			      ;
      movl	%%eax, %%cr4			      ;
      movl	%%cr0, %%eax			      ;
      orl		$0x80000000, %%eax		   ;
      movl	%%eax, %%cr0			      "
      :
      :
      : "eax"
	);
}
