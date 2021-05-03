#define ASM     1
#include "asmWrap.h"

// General wrapper for interrupt functions
// Saves registers/flags, calls function, then restores registers

#define generalWrap(name, functionCalled) \
    .global name ;\
    name: ;\
        pushl %esp ;\
        pushl %ebp ;\
        pushl %eax ;\
        pushl %ecx ;\
        pushl %edi ;\
        pushl %edx ;\
        pushl %ebx ;\
        pushl %esi ;\
        pushfl ;\
        call functionCalled ;\
        popfl ;\
        popl %esi ;\
        popl %ebx ;\
        popl %edx ;\
        popl %edi ;\
        popl %ecx ;\
        popl %eax ;\
        popl %ebp ;\
        popl %esp ;\
        iret ;\

generalWrap(pitASMWrap, pit_handler);
generalWrap(keyboardASMWrap, keyboard_handler);
generalWrap(rtcASMWrap, rtc_handler);
