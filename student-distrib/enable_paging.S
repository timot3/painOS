#define ASM     1

.globl edit_paging_flags
.globl tlb_flush
.align 4
edit_paging_flags:

  pushl %ebp
  movl  %esp, %ebp
  movl  8(%ebp), %eax    
  // load cr3 with page directory address & flush tlb 
  movl %eax, %cr3

  // set wp bit in cr4
  movl %cr4, %eax
  orl  $0x00000010, %eax 
  movl %eax, %cr4

  // set protection bits in cr3
  movl %cr0, %eax
  orl  $0x80000001, %eax  
  movl %eax, %cr0

  leave
  ret

# https://wiki.osdev.org/TLB
# Flush the transaction lookaside buffer.
tlb_flush:
	movl	%cr3,%eax
	movl	%eax,%cr3
  ret
