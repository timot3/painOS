#include"scheduling.h"
#include "syscall.h"
#include "rtc.h"
#include "paging.h"
#include "lib.h"
#define PROGRAM_NAME "shell"

volatile uint8_t curr_process = 2;

/*
 * switch_task
 *   DESCRIPTION: Switches from the currently running task to the next task.
 *                  Runs every time PIT sends an interrupt
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
void switch_task(uint32_t fl,uint32_t esi,uint32_t ebx,uint32_t edx,uint32_t edi,uint32_t ecx,uint32_t eax,uint32_t ebp,uint32_t esp,uint32_t eip) {
    // Using https://wiki.osdev.org/Scheduling_Algorithms and
    // https://wiki.osdev.org/User:Mariuszp/Scheduler_Tutorial as reference
    // for (i = 0; i < 0xFFFFFFE; i++);
    if(curr_pids[2] == 0) {
        printf("SCEDUULING 2\n");
        terminal_switch(3);
        execute((uint8_t*)PROGRAM_NAME);
        return;
    }

    if(curr_pids[1] == 0) {
        printf("SCEDUULING 1\n");
        terminal_switch(2);
        execute((uint8_t*)PROGRAM_NAME);
        return;
    }

    if(curr_pids[0] == 0) {
        printf("SCEDUULING 0\n");
        terminal_switch(1);
        execute((uint8_t*)PROGRAM_NAME);
        return;
    }

    printf("SWITCHING TASK... (process %d)\n", curr_process);

    // get the current PID that we will switch from
    // it is stored in the terminal struct array at the idx of current process
    uint8_t curr_pid = curr_pids[curr_process];
    // get the PCB of the current pid
    pcb_t *curr_pcb = get_pcb_addr(curr_pid);


    // save the registers of the pcb
    curr_pcb->registers.fl = fl;
    curr_pcb->registers.esi = esi;
    curr_pcb->registers.ebx = ebx;
    curr_pcb->registers.edx = edx;
    curr_pcb->registers.edi = edi;
    curr_pcb->registers.ecx = ecx;
    curr_pcb->registers.eax = eax;
    curr_pcb->registers.eip = eip;

    // get the next process ID and next PCB
    uint8_t new_process = (curr_process + 1) % 3;
    uint8_t new_pid = curr_pids[new_process];

    // if (new_pid == 0) {
    //     terminal_switch(2);
    //     execute((uint8_t*)"shell");
    //     // terminal_switch(curr_pid);

    // }

    // switch current process to next
    pcb_t *new_pcb = get_pcb_addr(new_pid);
    // store ESP0, ESP, EBP into the current PCB
    curr_pcb->esp0 = tss.esp0;
    asm volatile (
        "movl %%esp, %0;"
        "movl %%ebp, %1;"
        : "=g"(curr_pcb->parent.ksp), "=g"(curr_pcb->parent.kbp)
    );

    // set esp0, ss0 in the tss
    tss.esp0 = new_pcb->esp0;
    tss.ss0 = KERNEL_DS;

    // esp, ebp <-- new PCB esp, ebp
    asm volatile (
        "movl %0, %%esp;"
        "movl %1, %%ebp;"
        :
        : "g" (new_pcb->parent.ksp), "g" (new_pcb->parent.kbp)
    );

    // remap virtual page table of new task to the
    // physical memory of currently executing process
    // also flush tlb (inside the function)
    map_page_pid(new_pid);

    // Update next process
    curr_process = new_process;

    // Set register values based on new PCB saved values
    asm volatile (
        "movl %0, %%esi;"
        "movl %1, %%ebx;"
        "movl %2, %%edx;"
        "movl %3, %%edi;"
        "movl %4, %%ecx;"

        "pushl %6;"
        "popl %%eax;"
        "orl $0x200, %%eax;"
        "pushl %%eax;"
        "movl %7, %%eax;"
        "pushl %8;"
        "pushl %5;"

        "iret;"
        :
        : "g" (new_pcb->registers.esi), "g" (new_pcb->registers.ebx), "g" (new_pcb->registers.edx),
          "g" (new_pcb->registers.edi), "g" (new_pcb->registers.ecx), "g" (new_pcb->registers.eip),
          "g" (new_pcb->registers.fl),"g" (new_pcb->registers.eax), "g" (USER_CS)
    );
}
