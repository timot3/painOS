#include "idt.h"
#include "lib.h"
#include "x86_desc.h"
#include "keyboard.h"
#include "rtc.h"

void idt_error()
{
    printf("-----------interrupt has occurred--------------\n");
    while (1)
        ;
}

void DIV_BY_ZERO()
{
    printf("-----------DIV_BY_ZERO--------------\n");
    // while(1);
}

void RESERVED_INT(){
    printf("-----------RESERVED_INT--------------\n");
    while(1);
}

void NMI_INTERRUPT(){
    printf("-----------NMI_INTERRUPT--------------\n");
    while(1);
}

void BREAKPOINT(){
    printf("-----------BREAKPOINT--------------\n");
    while(1);
}

void OVERFLOW(){
    printf("-----------OVERFLOW--------------\n");
    while(1);
}

void BOUND_RANGE_EXCEEDED(){
    printf("-----------BOUND_RANGE_EXCEEDED--------------\n");
    while(1);
}

void INVALID_OPCODE(){
    printf("-----------INVALID_OPCODE--------------\n");
    while(1);
}

void DEVICE_NOT_AVAILABLE(){
    printf("-----------DEVICE_NOT_AVAILABLE--------------\n");
    while(1);
}

void DOUBLE_FAULT(){
    printf("-----------DOUBLE_FAULT--------------\n");
    while(1);
}

void COPROCESSOR_SEGMENT_OVERRUN(){
    printf("-----------COPROCESSOR_SEGMENT_OVERRUN--------------\n");
    while(1);
}

void INVALID_TSS(){
    printf("-----------INVALID_TSS--------------\n");
    while(1);
}

void SEGMENT_NOT_PRESENT(){
    printf("-----------SEGMENT_NOT_PRESENT--------------\n");
    while(1);
}

void STACK_SEGMENT_FAULT(){
    printf("-----------STACK_SEGMENT_FAULT--------------\n");
    while(1);
}

void GENERAL_PROTECTION(){
    printf("-----------GENERAL_PROTECTION--------------\n");
    while(1);
}

void PAGE_FAULT(){
    printf("-----------PAGE_FAULT--------------\n");
    while(1);
}

void INTEL_RESERVED(){
    printf("-----------INTEL_RESERVED--------------\n");
    while(1);
}

void MATH_FAULT(){
    printf("-----------MATH_FAULT--------------\n");
    while(1);
}

void ALIGNMENT_CHECK(){
    printf("-----------ALIGNMENT_CHECK--------------\n");
    while(1);
}

void MACHINE_CKECK(){
    printf("-----------MACHINE_CKECK--------------\n");
    while(1);
}

void SIMD_FLOATING_POINT_EXCEPTION(){
    printf("-----------SIMD_FLOATING_POINT_EXCEPTION--------------\n");
    while(1);
}

void initialize_idt()
{
    int i;

    // Set values for all elements in IDT
    for (i = 0; i < NUM_VEC; i++)
    {
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
        if (i >= 32)
        {
            idt[i].dpl = 3;
        }
    }

    for(i = 0; i < NUM_SYSTEM_INTERRUPTS; i++) {
        // Need to have different functions for each interrupt to print
        // different messages, but for now a single function works
        idt[i].reserved3 = 1; // enable for first 32, use trap gate
        idt[i].present = 1;
        if (i > 19) {
            SET_IDT_ENTRY(idt[i], idt_error);
        }
    }



    SET_IDT_ENTRY(idt[0], DIV_BY_ZERO);
    SET_IDT_ENTRY(idt[1], RESERVED_INT);
    SET_IDT_ENTRY(idt[2], NMI_INTERRUPT);
    SET_IDT_ENTRY(idt[3], BREAKPOINT);
    SET_IDT_ENTRY(idt[4], OVERFLOW);
    SET_IDT_ENTRY(idt[5], BOUND_RANGE_EXCEEDED);
    SET_IDT_ENTRY(idt[6], INVALID_OPCODE);
    SET_IDT_ENTRY(idt[7], DEVICE_NOT_AVAILABLE);
    SET_IDT_ENTRY(idt[8], DOUBLE_FAULT);
    SET_IDT_ENTRY(idt[9], COPROCESSOR_SEGMENT_OVERRUN);
    SET_IDT_ENTRY(idt[10], INVALID_TSS);
    SET_IDT_ENTRY(idt[11], SEGMENT_NOT_PRESENT);
    SET_IDT_ENTRY(idt[12], STACK_SEGMENT_FAULT);
    SET_IDT_ENTRY(idt[13], GENERAL_PROTECTION);
    SET_IDT_ENTRY(idt[14], PAGE_FAULT);
    SET_IDT_ENTRY(idt[15], INTEL_RESERVED);
    SET_IDT_ENTRY(idt[16], MATH_FAULT);
    SET_IDT_ENTRY(idt[17], ALIGNMENT_CHECK);
    SET_IDT_ENTRY(idt[18], MACHINE_CKECK);
    SET_IDT_ENTRY(idt[19], SIMD_FLOATING_POINT_EXCEPTION);

    SET_IDT_ENTRY(idt[RTC_NUM], test_interrupts);
    SET_IDT_ENTRY(idt[KB_NUM],keyboard_handler);

    
    idt[RTC_NUM].present = 1;
    idt[KB_NUM].present = 1;


    // Load IDT after initialization
    lidt(idt_desc_ptr);
}
