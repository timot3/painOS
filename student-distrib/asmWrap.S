#define ASM     1
#include "asmWrap.h"

// General wrapper for interrupt functions
// Saves registers, calls function, then restores registers

#define generalWrap(name, functionCalled) \
    .global name ;\
    name: ;\
        pushl %ebp ;\
        pushl %eax ;\
        pushl %ecx ;\
        pushl %edi ;\
        pushl %edx ;\
        pushl %ebx ;\
        pushl %esi ;\
        call functionCalled ;\
        popl %esi ;\
        popl %ebx ;\
        popl %edx ;\
        popl %edi ;\
        popl %ecx ;\
        popl %eax ;\
        popl %ebp ;\
        iret ;\

generalWrap(rtcASMWrap, rtc_handler);
generalWrap(keyboardASMWrap, keyboard_handler);
