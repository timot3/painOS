#ifndef IDT_H
#define IDT_H

// Based on values given in table from coursenotes
#define PIT_NUM 0x20
#define KB_NUM 0x21
#define RTC_NUM 0x28

#define SYSCALL_NUM 0x80
#define NUM_SYSTEM_INTERRUPTS 32
#define EXIT_STATUS ((uint8_t)256)

// Generic IDT handler function
void idt_error();

// Handler functions for first 20 IDT values (plus some more)
void DIV_BY_ZERO();
void RESERVED_INT();
void NMI_INTERRUPT();
void BREAKPOINT();
void OVERFLOW();
void BOUND_RANGE_EXCEEDED();
void INVALID_OPCODE();
void DEVICE_NOT_AVAILABLE();
void DOUBLE_FAULT();
void COPROCESSOR_SEGMENT_OVERRUN();
void INVALID_TSS();
void SEGMENT_NOT_PRESENT();
void STACK_SEGMENT_FAULT();
void GENERAL_PROTECTION();
void PAGE_FAULT();
void INTEL_RESERVED();
void MATH_FAULT();
void ALIGNMENT_CHECK();
void MACHINE_CHECK();
void SIMD_FLOATING_POINT_EXCEPTION();
void SYSTEM_CALL();

// Function to initialize IDT
void initialize_idt();

#endif /* IDT_H */
