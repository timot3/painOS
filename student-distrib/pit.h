#ifndef PIT_H
#define PIT_H

#endif // PIT_H

#define PIT_IRQ 0
#define CHANNEL_0_DATA 0x40
#define MODE_REGISTER 0x43
#define TWO_BYTE_MASK 0xFF
#define REPEAT_MODE 0x36
#define INPUT_CLK 1193180
// use default as 40
#define DESIRED_FREQUENCY 40

void initialize_pit();
void pit_handler();
