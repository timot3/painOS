#ifndef SCHEDULING_H
#define SCHEDULING_H
#include "lib.h"
void switch_task(uint32_t fl,uint32_t esi,uint32_t ebx,uint32_t edx,uint32_t edi,uint32_t ecx,uint32_t eax,uint32_t ebp,uint32_t esp,uint32_t eip);
#define PROGRAM_IMAGE_START 0x8000000 // 128 MB

#endif // SCHEDULING.H


