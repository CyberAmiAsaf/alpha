#include "../hd.h"

CHS LBA2CHS(unsigned int lba) {
  unsigned int cyl = lba/(H*S);
  unsigned int head = (lba%(H*S))/S;
  unsigned int sect = (lba%(H*S))%S+1;

  CHS chs = {
    cyl,
    head,
    sect
  };

  return chs;
}


void hd_rw(unsigned int lba, unsigned int command, unsigned int sects_to_access, void* buf) {
  CHS chs = LBA2CHS(lba);

  // Structure of the `Status Register Output`
  // Bit 0  previous command ended in an error
  // Bit 1  index - set to 1 each revolution
  // Bit 2  disk data read corrected
  // Bit 3  sector buffer requires servicing
  // Bit 4  seek completed
  // Bit 5  write error
  // Bit 6  drive is ready
  // Bit 7  controller is busy

  //0xc0 = 1100 0000 -> only looks at the `drive is ready` and `controller is busy` flags
  //0x40 = 0100 0000 -> desired state, only the `drive is ready` flag is lit up

  // Wait until drive is ready
  while((port_byte_in(HD_PORT_STATUS)&0xc0) != 0x40);


  //The drive is ready!
  port_byte_out(HD_PORT_SECT_COUNT, sects_to_access);
  port_byte_out(HD_PORT_SECT_NUM, chs.sect);
  port_byte_out(HD_PORT_CYL_LOW, chs.cyl);
  port_byte_out(HD_PORT_CYL_HIGH, chs.cyl >> 8);

  //Structure of the `HD_PORT_DRV_HEAD` input
  // Bits 7-5  must be 101b
  // Bit 4  HD0(=0), HD1(=1)
  // Bits 3-0  head number

  //0xa0 = 1010 0000 -> light up the `101` flags at the end of the byte
  port_byte_out(HD_PORT_DRV_HEAD,  0b10110000| chs.head);

  //Send the command to the command port
  port_byte_out(HD_PORT_COMMAND, command);

  while (!(port_byte_in(HD_PORT_STATUS)&0x8));

  if (command == HD_READ) {
    port_sl_in(HD_PORT_DATA, buf, sects_to_access<<7);
  } else if(command == HD_WRITE) {
    port_sl_out(HD_PORT_DATA, buf, sects_to_access<<7);
  }
}


void setup_dpt(void) {
  unsigned char sect[512];
  hd_rw(0, HD_READ, 1, sect);
  sect[0x1be] = 0x80;
  sect[0x1be + 0x04] = FST_FS;
  *(unsigned long *)&sect[0x1be + 0x08] = 1;

  // 2GB  = ((512 bytes per sector)*2 = 1024)*1024*1024*2
  *(unsigned long *)&sect[0x1be + 0x0c] = 1024*2*1024*2;
  sect[0x1ce + 0x04] = FST_SW;
  *(unsigned long *)&sect[0x1ce + 0x08] = 1024*2*1024*2+1;

  // 1GB ((512 bytes per sector)*2 = 1024)*1024*1024 */
  *(unsigned long *)&sect[0x1ce + 0x0c] = 1024*1024*2;

  // magic mbr 0xaa55
  sect[0x1fe] = 0x55;
  sect[0x1ff] = 0xaa;
  hd_rw(0, HD_WRITE, 1, sect);
}


bool verify_dpt(void) {
  extern unsigned int* hd0;

  unsigned char sect[512];
  unsigned i = 0;
  unsigned int *q = (unsigned int *)(hd0);

  hd_rw(0, HD_READ, 1, sect);

  return (bool)((sect[0x1fe] == 0x55) && (sect[0x1ff] == 0xaa) && (sect[0x1be + 0x04] == FST_FS) && (sect[0x1ce + 0x04] == FST_SW));
}

void print_hd_table () {
  extern unsigned int* hd0;

  unsigned char sect[512];
  unsigned i = 0;
  unsigned int *q = (unsigned int *)(hd0);

  hd_rw(0, HD_READ, 1, sect);

  if (verify_dpt()) {
    unsigned char* p = &sect[0x1be];
    char* s;
    printf("   | Bootable | Type      | Start Sector | Capacity \n");
    for(i = 0; i < 4; i++) {
      char tmp[2];
      itoa(i, tmp);
      printf(tmp);

      if (p[0x04] == 0x00) {
        printf("  | Empty\n");
        p += 16;
        q += 2;
        continue;
      }

      if (p[0x00] == 0x80)
        s = "  | Yes      ";
      else
        s = "  | No       ";

      printf(s);

      /* system indicator at offset 0x04 */
      if (p[0x04] == FST_FS) {
        printf("| ALPHA FS ");
      } else if (p[0x04] == FST_SW) {
        printf("| ALPHA SW ");
      } else {
        printf("| Unknown   ");
      }


      /* starting sector number */
      *q++ = *(unsigned long *) &p[0x08];
      printf(" | ");
      char starting_sector_number[get_num_digits(*(unsigned long *)&p[0x08]) + 1];
      itoa(*(unsigned long *) &p[0x08], starting_sector_number);
      printf(starting_sector_number);

      /* capacity */
      *q++ = *(unsigned long *) &p[0x0c];
      printf("            | ");
      char capacity[get_num_digits(*(unsigned long *)&p[0x0c]) + 1];
      itoa((*(unsigned long *) &p[0x0c]*512) >> 20, capacity);
      printf(capacity);
      printf("MB");
      printf("\n");

      p += 16;
    }
  }
}
