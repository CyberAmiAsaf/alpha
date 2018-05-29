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

void hd_read(unsigned int lba, unsigned int sects_len, void *buf) {
  hd_rw(lba, HD_READ, sects_len, buf);
}

void hd_write(unsigned int lba, unsigned int sects_len, void *buf) {
  hd_rw(lba, HD_WRITE, sects_len, buf);
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

void init_alpha_hd(void) {
  char first_sect[512] = "alpha_os";

  hd_write(0, 1, first_sect);
}

bool verify_alpha_hd(void) {
  char first_sect[512];

  hd_read(0, 1, first_sect);

  return strncmp(first_sect, "alpha_os", 8) == 0;
}
