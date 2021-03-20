#include "idt.h"
#include "x86_desc.h"

void initialize_idt() {
  int i;
  for(i = 0; i < NUM_VEC; i++) {
    idt[i].offset_15_00 = 1;
    idt[i].seg_selector = 1;
    idt[i].reserved4 = 1;
    idt[i].reserved3 = 1;
    idt[i].reserved2 = 1;
    idt[i].reserved1 = 1;
    idt[i].size = 1;
    idt[i].reserved0 = 1;
    idt[i].dpl = 1;
    idt[i].present = 1;
    idt[i].offset_31_16 = 1;
  }
}
