#define ASM 1

.globl syscall_help

syscall_help:
    # save registers
    # do we need this?
    pushl %ebp
    movl  %ebp, %esp
    pushl %edi
    pushl %esi
    pushfl

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
    call *syscall_table(, %eax, 4)

    # restore argument registers
    popl %ebx
    popl %ecx
    popl %edx

    jmp return_syscall

oob_syscall:
    movl $-1, %eax
return_syscall:
    # restore other registers
    popfl
    popl %esi
    popl %edi
    popl %ebp
    # move -1 into eax, then return from int
    leave
    iret    
syscall_table:
    .long halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn
