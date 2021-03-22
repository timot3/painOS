#define ASM     1

.globl edit_paging_flags
.align 4
edit_paging_flags:
  pushl %ebp
  movl  %esp, %ebp
  movl  8(%ebp), %eax    
    
  movl %eax, %cr3

  movl %cr4, %eax
  orl  $0x00000010, %eax 
  movl %eax, %cr4

  movl %cr0, %eax
  orl  $0x80000001, %eax  
  movl %eax, %cr0

  leave
  ret


