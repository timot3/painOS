#ifndef IDT_H
#define IDT_H

void idt_error();

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
void MACHINE_CKECK();
void SIMD_FLOATING_POINT_EXCEPTION();
void initialize_idt();

void initialize_idt();

#define NUM_SYSTEM_INTERRUPTS 32

#endif /* IDT_H */
