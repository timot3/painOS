#include "idt.h"
#include "lib.h"
#include "x86_desc.h"

func_ptr allFunctionPointers[20] = {divByZeroInterrupt,
                                    reservedInterrupt,
                                    nmiInterrupt,
                                    breakpointInterrupt,
                                    overflowInterrupt,
                                    boundRangeExceededInterrupt,
                                    invalidOpcodeInterrupt,
                                    deviceNotAvailableInterrupt,
                                    doubleFaultInterrupt,
                                    coprocessorSegmentOverrunInterrupt,
                                    invalidTssInterrupt,
                                    segmentNotPresentInterrupt,
                                    stackSegmentFaultInterrupt,
                                    generalProtectionInterrupt,
                                    pageFaultInterrupt,
                                    intelReservedInterrupt,
                                    mathFaultInterrupt,
                                    alignmentCheckInterrupt,
                                    machineCheckInterrupt,
                                    simdFloatingPointExceptionInterrupt
};

void divByZeroInterrupt() {
    printf("-----------division by zero has occurred--------------\n");
    while(1);
}

void reservedInterrupt() {
    printf("-----------reserved interrupt has occurred--------------\n");
    while(1);
}

void nmiInterrupt() {
    printf("-----------NMI interrupt has occurred--------------\n");
    while(1);
}

void breakpointInterrupt() {
    printf("-----------breakpoint interrupt has occurred--------------\n");
    while(1);
}

void overflowInterrupt() {
    printf("-----------overflow interrupt has occurred--------------\n");
    while(1);
}

void boundRangeExceededInterrupt() {
    printf("-----------bound range exceeded error--------------\n");
    while(1);
}

void invalidOpcodeInterrupt() {
    printf("-----------invalid opcode error--------------\n");
    while(1);
}

void deviceNotAvailableInterrupt() {
    printf("-----------device not available error--------------\n");
    while(1);
}

void doubleFaultInterrupt() {
    printf("-----------double fault error--------------\n");
    while(1);
}

void coprocessorSegmentOverrunInterrupt() {
    printf("-------coprocessor segment overrun error----------\n");
    while(1);
}

void invalidTssInterrupt() {
    printf("-----------invalid TSS error--------------\n");
    while(1);
}

void segmentNotPresentInterrupt() {
    printf("-----------segment not present error--------------\n");
    while(1);
}

void stackSegmentFaultInterrupt() {
    printf("-----------stack segment fault error--------------\n");
    while(1);
}

void generalProtectionInterrupt() {
    printf("-----------general profection error--------------\n");
    while(1);
}

void pageFaultInterrupt() {
    printf("-----------page fault error--------------\n");
    while(1);
}

void intelReservedInterrupt() {
    printf("-----------Intel reserved error--------------\n");
    while(1);
}

void mathFaultInterrupt() {
    printf("-----------math fault error--------------\n");
    while(1);
}

void alignmentCheckInterrupt() {
    printf("-----------alignment check error--------------\n");
    while(1);
}

void machineCheckInterrupt() {
    printf("-----------machine check error--------------\n");
    while(1);
}

void simdFloatingPointExceptionInterrupt() {
    printf("-----------SIMD floating point exception error--------------\n");
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
        idt[i].dpl = 0; // Some devices might need 3 here instead of 0
        idt[i].present = 0;
    }

    // Set interrupt methods for first 32 values in IDT
    for(i = 0; i < NUM_SYSTEM_INTERRUPTS; i++) {
        idt[i].present = 1;

        // 20-31 needs to be Intel exception
        if(i < 20)
            SET_IDT_ENTRY(idt[i], allFunctionPointers[i]);
        else
            SET_IDT_ENTRY(idt[i], allFunctionPointers[15]);
    }

    // Load IDT after initialization
    lidt(idt_desc_ptr);
}
