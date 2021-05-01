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
void switch_task() {
    // Using https://wiki.osdev.org/Scheduling_Algorithms and
    // https://wiki.osdev.org/User:Mariuszp/Scheduler_Tutorial as reference
    // for (i = 0; i < 0xFFFFFFE; i++);

    if(curr_pids[2] == -1) {
        terminal_switch(3);
        printf("SCEDUULING 2\n");
        execute((uint8_t*)PROGRAM_NAME);
        return;
    }

    if(curr_pids[1] == -1) {
        terminal_switch(2);
        printf("SCEDUULING 1\n");
        execute((uint8_t*)PROGRAM_NAME);
        return;
    }

    if(curr_pids[0] == -1) {
        terminal_switch(1);
        printf("SCEDUULING 0\n");
        execute((uint8_t*)PROGRAM_NAME);
        return;
    }
    // // get the current PID that we will switch from
    // // it is stored in the terminal struct array at the idx of current process
    // int8_t curr_pid = curr_pids[curr_process];
    // // get the PCB of the current pid
    // pcb_t *curr_pcb = get_pcb_addr(curr_pid);

    // // get the next process ID and next PCB
    // uint8_t new_process = (curr_process + 1) % 3;
    // int8_t new_pid = curr_pids[new_process];

    // // terminal_switch(new_process + 1);
    // curr_process = new_process;

    // // switch current process to next
    // pcb_t *new_pcb = get_pcb_addr(new_pid);
    // // store ESP0, ESP, EBP into the current PCB
    // curr_pcb->esp0 = tss.esp0;
    // asm volatile (
    //     "movl %%esp, %0;"
    //     "movl %%ebp, %1;"
    //     : "=g"(curr_pcb->parent.ksp), "=g"(curr_pcb->parent.kbp)
    // );

    // // set esp0, ss0 in the tss
    // tss.esp0 = new_pcb->esp0;
    // tss.ss0 = KERNEL_DS;

    // // esp, ebp <-- new PCB esp, ebp
    // asm volatile (
    //     "movl %0, %%esp;"
    //     "movl %1, %%ebp;"
    //     :
    //     : "g" (new_pcb->parent.ksp), "g" (new_pcb->parent.kbp)
    // );

    // // remap virtual page table of new task to the
    // // physical memory of currently executing process
    // // also flush tlb (inside the function)
    // map_page_pid(new_pid);
}
