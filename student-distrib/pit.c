#include "pit.h"
#include "i8259.h"
#include "lib.h"
#include "scheduling.h"

/*
 * initialize_pit
 *   DESCRIPTION: Initializes the PIT
 *   INPUTS: none
 *   OUTPUTS: makes PIT work
 *   RETURN VALUE: none
 */
void initialize_pit() {
    // Using http://www.jamesmolloy.co.uk/tutorial_html/5.-IRQs%20and%20the%20PIT.html
    // and https://wiki.osdev.org/Programmable_Interval_Timer as reference

    // Get divisor rate based on desired frequency
    uint32_t divisor = INPUT_CLK / DESIRED_FREQUENCY; // every 25ms (40 Hz)

    // Send command byte (repeat mode)
    outb(REPEAT_MODE, MODE_REGISTER);

    // Send frequency divisor to PIT
    uint8_t l = divisor & TWO_BYTE_MASK;
    uint8_t h = (divisor >> 8) & TWO_BYTE_MASK;
    outb(l, CHANNEL_0_DATA);
    outb(h, CHANNEL_0_DATA);

    // Enable interrupt requests for PIT on PIC
    enable_irq(PIT_IRQ);
}

/*
 * pit_handler
 *   DESCRIPTION: Called every time PIT sends interrupt command
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
void pit_handler(uint32_t fl,uint32_t esi,uint32_t ebx,uint32_t edx,uint32_t edi,uint32_t ecx,uint32_t eax,uint32_t ebp,uint32_t esp,uint32_t eip) {
    // Send EOI so device knows we're done
    send_eoi(PIT_IRQ);
    
    // Call scheduling switch task function every time PIT sends interrupt
    switch_task(fl,esi,ebx,edx,edi,ecx,eax,ebp,esp,eip);

}
