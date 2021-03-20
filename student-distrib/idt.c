#include "idt.h"
#include "lib.h"
#include "x86_desc.h"

void idt_error() {
    printf("-----------interrupt has occurred--------------\n");
    while (1);
}
void initialize_idt() {
    int i;
    for (i = 0; i < NUM_SYSTEM_INTERRUPTS; i++) {
        if(i == RESERVED || i == INTEL_RESERVED) continue; // ignore reserved
        idt[i].seg_selector = KERNEL_CS;
        SET_IDT_ENTRY(idt[i], idt_error);
    }
    
    // SET_IDT_ENTRY(idt[i],DIV_BY_ZERO);
    // SET_IDT_ENTRY(idt[i],RESERVED);
    // SET_IDT_ENTRY(idt[i],NMI_INTERRUPT);
    // SET_IDT_ENTRY(idt[i],BREAKPOINT);
    // SET_IDT_ENTRY(idt[i],OVERFLOW);
    // SET_IDT_ENTRY(idt[i],BOUND_RANGE_EXCEEDED);
    // SET_IDT_ENTRY(idt[i],INVALID_OPCODE);
    // SET_IDT_ENTRY(idt[i],DEVICE_NOT_AVAILABLE);
    // SET_IDT_ENTRY(idt[i],DOUBLE_FAULT);
    // SET_IDT_ENTRY(idt[i],COPROCESSOR_SEGMENT_OVERRUN);
    // SET_IDT_ENTRY(idt[i],INVALID_TSS);
    // SET_IDT_ENTRY(idt[i],SEGMENT_NOT_PRESENT);
    // SET_IDT_ENTRY(idt[i],STACK_SEGMENT_FAULT);
    // SET_IDT_ENTRY(idt[i],GENERAL_PROTECTION);
    // SET_IDT_ENTRY(idt[i],PAGE_FAULT);
    // SET_IDT_ENTRY(idt[i],INTEL_RESERVED);
    // SET_IDT_ENTRY(idt[i],MATH_FAULT);
    // SET_IDT_ENTRY(idt[i],ALIGNMENT_CHECK);
    // SET_IDT_ENTRY(idt[i],MACHINE_CKECK);
    // SET_IDT_ENTRY(idt[i],SIMD_FLOATING_POINT_EXCEPTION);
}
