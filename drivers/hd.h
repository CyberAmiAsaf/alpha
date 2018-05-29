#ifndef ALPHA_HD_H
#define ALPHA_HD_H

#include <util.h>
#include <ports.h>

#define C 208
#define H 16
#define S 63

#define HD_PORT_DATA            0x1f0
#define HD_PORT_ERROR           0x1f1
#define HD_PORT_SECT_COUNT      0x1f2
#define HD_PORT_SECT_NUM        0x1f3
#define HD_PORT_CYL_LOW         0x1f4
#define HD_PORT_CYL_HIGH        0x1f5
#define HD_PORT_DRV_HEAD        0x1f6
#define HD_PORT_STATUS          0x1f7
#define HD_PORT_COMMAND         0x1f7

// Different Commands Available
// HD_READ=0x20 reading with retry if failed
// HD_WRITE=0x30 writing with retry if failed

#define HD_READ 0x20
#define HD_WRITE 0x30

//Different Types of File Systems
#define FST_FS 0x2e /* normal partition */
#define FST_SW 0x2f /* swap partition */

typedef struct HD_PARAM {
    unsigned int cyl;
    unsigned int head;
    unsigned int sect;
} CHS;

CHS LBA2CHS(unsigned int LBA); //Linear Block Addressing to Cylinders Headers and Sectors

void hd_read(unsigned int LBA, unsigned int sects_to_access, void* buf);
void hd_write(unsigned int LBA, unsigned int sects_to_access, void* buf);
void hd_rw(unsigned int LBA, unsigned int command, unsigned int sects_to_access, void* buf);

void init_alpha_hd(void);
bool verify_alpha_hd(void);
#endif
