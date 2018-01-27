#ifndef ALPHA_RTC_H
#define ALPHA_RTC_H

#include <types.h>

#include <ports/ports.h>
#include <isr/isr.h>
#include <util/util.h>

#define CMOS_INDEX_PORT 0x70
#define CMOS_DATA_PORT 0x71

#define CMOS_NMI_BIT 0x40

#define NMI_ENABLE(data)    \
    ((data) | (CMOS_NMI_BIT))
#define NMI_DISABLE(data)   \
    ((data) & (~(CMOS_NMI_BIT)))

#define RTC_CUR_SEC 0x0
#define RTC_ALARM_SEC 0x1
#define RTC_CUR_MIN 0x2
#define RTC_ALARM_MIN 0x3
#define RTC_CUR_HOUR 0x4
#define RTC_ALARM_HOUR 0x5
#define RTC_DAY_OF_WEEK 0x6
#define RTC_DAY_OF_MONTH 0x7
#define RTC_MONTH 0x8
#define RTC_YEAR 0x9

#define STATUS_REG_A 0xA
#define STATUS_REG_A_UPDATE_IN_PROGRESS 0x80
#define STATUS_REG_B 0xB
#define STATUS_REG_C 0xC
#define STATUS_REG_D 0xD

#define CMOS_DIAGNOSTIC_STATUS 0xE
#define CMOS_DIAGNOSTIC_RTC_LOST_POWER 0x80

#define CMOS_SHUTDOWN_STATUS 0xF

#define BCD_TO_INT(bcd) \
    (((((bcd) & 0xF0) >> 4) * 10) + ((bcd) & 0xF))

/* Write data */
#define CMOS_DISKETTE 0x10
/* Read data */
#define CMOS_DISKETTE_NO_DRIVE 0x0
#define CMOS_DISKETTE_360k 0x1
#define CMOS_DISKETTE_1M2 0x2
#define CMOS_DISKETTE_720k 0x3
#define CMOS_DISKETTE_1M44 0x4
/* Checking */
#define CMOS_DISKETTE_TYPE_DRIVE0(data) \
    (((data) >> 4) & 0xF)
#define CMOS_DISKETTE_TYPE_DRIVE1(data) \
    ((data) & 0xF)

int cmos_init();
int rtc_get_time();
int cmos_get_flp_status();

#endif
