#define ASM 1

.globl syscall_help

syscall_help:
    # save registers
    # do we need this?
#    pushl %ebp
#    pushl %edi
#    pushl %esi

    # syscall bound test
    cmp $1, %eax
    jl oob_syscall
    cmp $10, %eax
    jg oob_syscall

   # Push argument registers
   pushl %edx
   pushl %ecx
   pushl %ebx

    # jmp table starts at 0, so fit eax to table
    decl %eax
    jmp *syscall_table(, %eax, 4)

# return_syscall:
    # restore argument registers
#    popl %ebx
#    popl %ecx
#    popl %edx


oob_syscall:
    # restore other registers
#    popl %esi
#    popl %edi
#    popl %ebp
    # move -1 into eax, then return from int
    movl $-1, %eax
    iret    
syscall_table:
    .long halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn
