#ifndef PAGING_H
#define PAGING_H

#include "types.h"

#define PAGE_TABLE_LENGTH 1024 
#define PAGE_DIRECTORY_LENGTH 1024
#define ADDRESS_SHIFT 12 // to shift to fit into aligned_address
#define KERNEL_LOCATION 0x400000 // for the kernel block
#define VID_MEM 0xB8 // for vid mem block

#define VIRT_PAGE_NUM 32
#define TASK_SIZE 0x400000
#define KERNEL_PAGE 0x800000

// PDE struct
typedef struct page_dir_entry
{
  // 4B, needs to accomodate mapped or unmapped
  union {
    uint32_t val;
    struct
    {
      char present : 1;       // 1 if present
      char rw : 1;            // 1 is rw, 0 ro
      char us : 1;            // 1 = user perms,  0=supervisor
      char write_through : 1; // write-through caching
      char cache_disable : 1; // 1 if disabled
      char accessed : 1;      // if accessed
      char zero : 1;          // the wiki says to have a zero bit
      char size : 1;          // 0 = 4kB, 1 = 4mB
      char ignored : 1;
      char available : 3;
      uint32_t aligned_address : 20;
    } __attribute__((packed));
  };
} page_dir_entry_t;

// PTE struct
typedef struct page_table_entry
{
  // 4B, needs to accomodate mapped or unmapped
  union {
    uint32_t val;
    struct
    {
      char present : 1;       // 1 if present
      char rw : 1;            // 1 is rw, 0 ro
      char us : 1;            // 1 = user perms,  0=supervisor
      char write_through : 1; // write-through caching
      char cache_disable : 1; // 1 if disabled
      char accessed : 1;      // if accessed
      char dirty : 1;         // dirty table entry, if written to
      char zero : 1;          // all hail our wiki lord and savior
      char global : 1;        // prevents tlb from updang cached address if cr3
      char available : 3;
      uint32_t aligned_address : 20;
    } __attribute__((packed));
  };
} page_table_entry_t;

// Initializes the paging function, including creating the struct array and 
// setting the Control Registers
extern void paging_init();

// flush the tlb
extern void tlb_flush(void);

// For test functions, copies paging directory
extern void get_paging_directory(page_dir_entry_t *page_dir_alt, int len);
// For test functions, copies a paging table
extern void get_paging_table(page_table_entry_t *page_table_alt, int len);

void map_page_pid(int pid);

#endif /* PAGING_H */
