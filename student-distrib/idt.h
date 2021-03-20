#ifndef IDT_H
#define IDT_H

#define NUM_SYSTEM_INTERRUPTS 32

typedef void (*func_ptr)();

void initialize_idt();

// Interrupt functions
void divByZeroInterrupt();
void reservedInterrupt();
void nmiInterrupt();
void breakpointInterrupt();
void overflowInterrupt();
void boundRangeExceededInterrupt();
void invalidOpcodeInterrupt();
void deviceNotAvailableInterrupt();
void doubleFaultInterrupt();
void coprocessorSegmentOverrunInterrupt();
void invalidTssInterrupt();
void segmentNotPresentInterrupt();
void stackSegmentFaultInterrupt();
void generalProtectionInterrupt();
void pageFaultInterrupt();
void intelReservedInterrupt();
void mathFaultInterrupt();
void alignmentCheckInterrupt();
void machineCheckInterrupt();
void simdFloatingPointExceptionInterrupt();

#endif /* IDT_H */
