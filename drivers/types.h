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

// access high/low word of double word
#define low_16(address) (u16)((address) & 0xFFFF)
#define high_16(address) (u16)(((address) >> 16) & 0xFFFF)

#endif
