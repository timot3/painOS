#include "idt.h"
#include "lib.h"
#include "x86_desc.h"
#include "keyboard.h"
#include "rtc.h"

#define PAIN_HEADER \
    printf("\n             _         \n");\
    printf(" _ __   __ _(_)_ __    \n");\
    printf("| '_ \\ / _` | | '_ \\   \n");\
    printf("| |_) | (_| | | | | |_ \n");\
    printf("| .__/ \\__,_|_|_| |_(_)\n");\
    printf("|_|                    \n")

void idt_error() {
    PAIN_HEADER;
    printf("-----------interrupt has occurred--------------\n");
    while(1);
}

void DIV_BY_ZERO() {
    PAIN_HEADER;
    printf("-----------DIV_BY_ZERO--------------\n");
    while(1);
}

void RESERVED_INT() {
    PAIN_HEADER;
    printf("-----------RESERVED_INT--------------\n");
    while(1);
}

void NMI_INTERRUPT() {
    PAIN_HEADER;
    printf("-----------NMI_INTERRUPT--------------\n");
    while(1);
}

void BREAKPOINT() {
    PAIN_HEADER;
    printf("-----------BREAKPOINT--------------\n");
    while(1);
}

void OVERFLOW() {
    PAIN_HEADER;
    printf("-----------OVERFLOW--------------\n");
    while(1);
}

void BOUND_RANGE_EXCEEDED() {
    PAIN_HEADER;
    printf("-----------BOUND_RANGE_EXCEEDED--------------\n");
    while(1);
}

void INVALID_OPCODE() {
    PAIN_HEADER;
    printf("-----------INVALID_OPCODE--------------\n");
    while(1);
}

void DEVICE_NOT_AVAILABLE() {
    PAIN_HEADER;
    printf("-----------DEVICE_NOT_AVAILABLE--------------\n");
    while(1);
}

void DOUBLE_FAULT() {
    PAIN_HEADER;
    printf("-----------DOUBLE_FAULT--------------\n");
    while(1);
}

void COPROCESSOR_SEGMENT_OVERRUN() {
    PAIN_HEADER;
    printf("-----------COPROCESSOR_SEGMENT_OVERRUN--------------\n");
    while(1);
}

void INVALID_TSS() {
    PAIN_HEADER;
    printf("-----------INVALID_TSS--------------\n");
    while(1);
}

void SEGMENT_NOT_PRESENT() {
    PAIN_HEADER;
    printf("-----------SEGMENT_NOT_PRESENT--------------\n");
    while(1);
}

void STACK_SEGMENT_FAULT() {
    PAIN_HEADER;
    printf("-----------STACK_SEGMENT_FAULT--------------\n");
    while(1);
}

void GENERAL_PROTECTION() {
    PAIN_HEADER;
    printf("-----------GENERAL_PROTECTION--------------\n");
    while(1);
}

void PAGE_FAULT() {
    PAIN_HEADER;
    printf("-----------PAGE_FAULT--------------\n");
    while(1);
}

void INTEL_RESERVED() {
    PAIN_HEADER;
    printf("-----------INTEL_RESERVED--------------\n");
    while(1);
}

void MATH_FAULT() {
    PAIN_HEADER;
    printf("-----------MATH_FAULT--------------\n");
    while(1);
}

void ALIGNMENT_CHECK() {
    PAIN_HEADER;
    printf("-----------ALIGNMENT_CHECK--------------\n");
    while(1);
}

void MACHINE_CKECK() {
    PAIN_HEADER;
    printf("-----------MACHINE_CKECK--------------\n");
    while(1);
}

void SIMD_FLOATING_POINT_EXCEPTION() {
    PAIN_HEADER;
    printf("-----------SIMD_FLOATING_POINT_EXCEPTION--------------\n");
    while(1);
}

void SYSTEM_CALL() {
    PAIN_HEADER;
    printf("-----------SYSTEM_CALL_OCCURED--------------\n");
    while(1);
}

void initialize_idt() {
    int i;

    // Set values for all elements in IDT
    for(i = 0; i < NUM_VEC; i++) {
        // Set based on values found at
        // http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html
        // and
        // https://courses.engr.illinois.edu/ece391/sp2021/secure/references/IA32-ref-manual-vol-3.pdf
        // pg 156
        idt[i].seg_selector = KERNEL_CS;
        idt[i].reserved4    = 0;
        idt[i].reserved3    = 0;
        idt[i].reserved2    = 1;
        idt[i].reserved1    = 1;
        idt[i].size         = 1;
        idt[i].reserved0    = 0;
        idt[i].dpl          = 0;
        idt[i].present      = 0;

        if(i >= NUM_SYSTEM_INTERRUPTS) {
            idt[i].dpl = 3; // Greater than 32 has user-level priority
        }
    }
    idt[32].dpl = 3; // Office hours TA said to do this

    // Change values for first 32 items in IDT
    for(i = 0; i < NUM_SYSTEM_INTERRUPTS; i++) {
        idt[i].reserved3 = 1; // enable for first 32, use trap gate
        idt[i].present   = 1;

        // interrupts 20-31 are intel-reserved per the documentation.
        if(i > 19) {
            SET_IDT_ENTRY(idt[i], INTEL_RESERVED);
        }
    }

    // init the idt
    //https://courses.engr.illinois.edu/ece391/sp2021/secure/references/IA32-ref-manual-vol-3.pdf see page 145 for vector numbers
    SET_IDT_ENTRY(idt[0],       DIV_BY_ZERO);
    SET_IDT_ENTRY(idt[1],       RESERVED_INT);
    SET_IDT_ENTRY(idt[2],       NMI_INTERRUPT);
    SET_IDT_ENTRY(idt[3],       BREAKPOINT);
    SET_IDT_ENTRY(idt[4],       OVERFLOW);
    SET_IDT_ENTRY(idt[5],       BOUND_RANGE_EXCEEDED);
    SET_IDT_ENTRY(idt[6],       INVALID_OPCODE);
    SET_IDT_ENTRY(idt[7],       DEVICE_NOT_AVAILABLE);
    SET_IDT_ENTRY(idt[8],       DOUBLE_FAULT);
    SET_IDT_ENTRY(idt[9],       COPROCESSOR_SEGMENT_OVERRUN);
    SET_IDT_ENTRY(idt[10],      INVALID_TSS);
    SET_IDT_ENTRY(idt[11],      SEGMENT_NOT_PRESENT);
    SET_IDT_ENTRY(idt[12],      STACK_SEGMENT_FAULT);
    SET_IDT_ENTRY(idt[13],      GENERAL_PROTECTION);
    SET_IDT_ENTRY(idt[14],      PAGE_FAULT);
    SET_IDT_ENTRY(idt[15],      INTEL_RESERVED);
    SET_IDT_ENTRY(idt[16],      MATH_FAULT);
    SET_IDT_ENTRY(idt[17],      ALIGNMENT_CHECK);
    SET_IDT_ENTRY(idt[18],      MACHINE_CKECK);
    SET_IDT_ENTRY(idt[19],      SIMD_FLOATING_POINT_EXCEPTION);


    // Enable keyboard, RTC, and system call by marking them present
    idt[RTC_NUM].present = 1;
    idt[KB_NUM].present  = 1;
    idt[SYSCALL_NUM].present = 1;

    // Add rtc handler,  keyboard, and system call to the idt
    SET_IDT_ENTRY(idt[RTC_NUM], rtc_handler);
    SET_IDT_ENTRY(idt[KB_NUM],  keyboard_handler);
    SET_IDT_ENTRY(idt[SYSCALL_NUM],  SYSTEM_CALL);


    // Load IDT after initialization
    lidt(idt_desc_ptr);
}
