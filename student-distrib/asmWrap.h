#ifndef ASMWRAP_H
#define ASMWRAP_H

// ASM interrupt function wrappers
#ifndef ASM
    void pitASMWrap();
    void keyboardASMWrap();
    void rtcASMWrap();
#endif

#endif
