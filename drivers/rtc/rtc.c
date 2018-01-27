#include "rtc.h"

/*
 * CMOS OUT port
 */
static int cmos_select_ram(int idx)
{
  port_byte_out(CMOS_INDEX_PORT, idx);
  return 0;
}

/*
 * CMOS IN port
 */
inline static char cmos_read_ram()
{
  return port_byte_in(CMOS_DATA_PORT);
}

inline static int cmos_write_ram(int data)
{
  port_byte_out(CMOS_DATA_PORT, data);
  return 0;
}

/*
 * Returns 1 if RTC update is in progress.
 * In such case returned time might be incorrect.
 */
static int rtc_in_update()
{
  int ram = NMI_DISABLE(STATUS_REG_A);

  cmos_select_ram(ram);
  char res = cmos_read_ram();

  return (res & STATUS_REG_A_UPDATE_IN_PROGRESS);
}

/* 
 * Checks if CMOS has any problems.
 * Returns 0 on pass.
 */
static int cmos_diagnostic()
{
  char ram;

  ram = CMOS_DIAGNOSTIC_STATUS;
  ram = NMI_DISABLE(ram);
  cmos_select_ram(ram);

  return cmos_read_ram();
}

/*
 * Returns the number of floppy drives attached to the PC.
 */
int cmos_get_flp_status()
{
  char ram;

  ram = NMI_DISABLE(CMOS_DISKETTE);
  cmos_write_ram(ram);
  cmos_select_ram(ram);
  return cmos_read_ram();
}

#define return_time(time)   \
  do {  \
    int ram = NMI_DISABLE(time);  \
    cmos_select_ram(ram);   \
    return BCD_TO_INT(cmos_read_ram()); \
  } while(0);

static int rtc_get_sec()
{
  return_time(RTC_CUR_SEC);
}
static int rtc_get_min()
{
  return_time(RTC_CUR_MIN);
}
static int rtc_get_hour()
{
  return_time(RTC_CUR_HOUR);
}
#undef return_time

/*
 * Returns time in an integer. Do shifting to extract specific values.
 * Bytes:
 *    0 - 7:  seconds
 *    8 -15:  minutes
 *    16-23:  hours
 *    24-31:  unused
 */
int rtc_get_time()
{
  int time = 0;

  /* busy loop while the RTC is updating itself */
  while (rtc_in_update());

  time |= rtc_get_sec();
  time |= (rtc_get_min() << 8);
  time |= (rtc_get_hour() << 16);

  return time;
}

/*
 * CMOS init.
 * Returns 0 on success.
 */
int cmos_init()
{
  int res;

  res = cmos_diagnostic();
  if (res)
  {
    /* if CMOS battery is not present, have to set the error bit off */
    if (res & CMOS_DIAGNOSTIC_RTC_LOST_POWER)
    {
      res &= ~CMOS_DIAGNOSTIC_RTC_LOST_POWER;
      cmos_select_ram(CMOS_DIAGNOSTIC_STATUS);
      cmos_write_ram(res);
    }
    else
      return -1; /* other diagnostic errors mean something not good */
  }

  return 0;
}


