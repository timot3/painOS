#include "paging.h"
extern void edit_paging_flags(int page_dir_addr);

// align with 4KB Chunks
page_dir_entry_t page_dir[PAGE_DIRECTORY_LENGTH] __attribute__((aligned(PAGE_DIRECTORY_LENGTH * 4)));
page_table_entry_t page_table[PAGE_TABLE_LENGTH] __attribute__((aligned(PAGE_TABLE_LENGTH * 4)));

void paging_init()
{
  int i;

  for (i = 0; i < PAGE_DIRECTORY_LENGTH; i++)
  {
    page_dir[i].val = 0; // clear contents
    // set to rw
    page_dir[i].rw = 1;
    page_dir[i].us = 1;
  }

  for (i = 0; i < PAGE_TABLE_LENGTH; i++)
  {
    page_table[i].val = 0; // clear contents
    // set to rw
    page_table[i].rw = 1;
    page_table[i].us = 1;
    // set address
    page_table[i].aligned_address = i;
  }

  // connect PDT to page table
  page_dir[0].aligned_address = ((int) &page_table) >> ADDRESS_SHIFT;
  page_dir[0].present = 1;
  page_dir[0].cache_disable = 1;
  page_dir[0].val = (int)&page_table | 0x17;

  // rw should probably be 0
  // set us?
  // set write_through?


  // connect 4-8 MB memory

  page_dir[1].aligned_address = KERNEL_LOCATION >> ADDRESS_SHIFT;
  page_dir[1].present = 1;
  page_dir[1].rw = 1;
  page_dir[1].size = 1; // 4 MB
  page_dir[1].val = 0x400083;

  // connect video memory 
  page_table[VID_MEM].aligned_address = 0xB8;
  page_table[VID_MEM].present = 1;
  page_table[VID_MEM].cache_disable = 1;
  page_table[184].val = 0xB8117;

  // WHY DOES THIS AND ONLY THIS WORK?
  edit_paging_flags((int)page_dir);
}


extern void get_paging_directory(page_dir_entry_t *page_dir_alt, int len){
  int i;
  for(i = 0; i<len; i++)
   {
      page_dir_alt[i]=page_dir[i];
   }
}
extern void get_paging_table(page_table_entry_t *page_table_alt, int len){
  int i;
  for(i = 0; i<len; i++)
   {
      page_table_alt[i]=page_table[i];
   }
}
