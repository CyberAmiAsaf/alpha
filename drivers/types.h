#ifndef ALPHA_TYPES_H
#define ALPHA_TYPES_H
 
// define boolean
typedef int bool;
#define true 1
#define false 0

// define size-related types
typedef unsigned int u32;
typedef int s32;
typedef unsigned short u16;
typedef short s16;
typedef unsigned char u8;
typedef char  s8;

// Struct which aggregates many registers
typedef struct {
   u32 ds; // Data segment selector
   u32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
   u32 int_no, err_code; // Interrupt number and error code (if applicable)
   u32 eip, cs, eflags, useresp, ss; // Pushed by the processor automatically
} registers_t; 

// access high/low word of double word
#define low_16(address) (u16)((address) & 0xFFFF)
#define high_16(address) (u16)(((address) >> 16) & 0xFFFF)

#endif
