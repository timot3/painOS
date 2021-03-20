#include "idt.h"
#include "lib.h"
#include "x86_desc.h"

void idt_error() {
    printf("-----------interrupt has occurred--------------\n");
    while(1);
}
void initialize_idt() {
    int i;

    // Set values for all elements in IDT
    for (i = 0; i < NUM_VEC; i++) {
        // Set based on values found at http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html
        // and https://courses.engr.illinois.edu/ece391/sp2021/secure/references/IA32-ref-manual-vol-3.pdf pg 156
        idt[i].seg_selector = KERNEL_CS;
        idt[i].reserved4 = 0;
        idt[i].reserved3 = 0;
        idt[i].reserved2 = 1;
        idt[i].reserved1 = 1;
        idt[i].size = 1;
        idt[i].reserved0 = 0;
        idt[i].dpl = 0;
        idt[i].present = 0;

        // // Set present for devices that always exist
        if(i < 32)
            idt[i].present = 1;
    }

    for(i = 0; i < 32; i++) {
        // Need to have different functions for each interrupt to print
        // different messages, but for now a single function works
        SET_IDT_ENTRY(idt[i], idt_error);
    }

    // Load IDT after initialization
    lidt(idt_desc_ptr);

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
