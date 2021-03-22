#define ASM     1
#include "asmWrap.h"

// General wrapper for interrupt functions
// Saves registers, calls function, then restores registers

#define generalWrap(name, functionCalled) \
    .global name ;\
    name: ;\
        pushl %ebp ;\
        pushl %eax ;\
        pushl %ebx ;\
        pushl %ecx ;\
        pushl %edx ;\
        pushl %edi ;\
        pushl %esi ;\
        call functionCalled ;\
        popl %esi ;\
        popl %edi ;\
        popl %edx ;\
        popl %ecx ;\
        popl %ebx ;\
        popl %eax ;\
        popl %ebp ;\
        iret ;\

generalWrap(rtcASMWrap, rtc_handler);
generalWrap(keyboardASMWrap, keyboard_handler);
