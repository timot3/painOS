#ifndef _SYSCALL_HELPER_H
#define _SYSCALL_HELPER_H
#include "syscall.h"


// ASM interrupt function wrappers
#ifndef ASM
    extern void syscall_help(); // not int because it returns in registers
#endif

#endif // _SYSCALL_HELPER_H