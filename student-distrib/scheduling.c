#include"scheduling.h"
#include "syscall.h"
#include "rtc.h"
#include "paging.h"
#include "lib.h"


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

    printf("SWITCHING TASK...\n");

    // Switch between current and next task using the round-robin method
}
