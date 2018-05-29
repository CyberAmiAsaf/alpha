  ;; =512
$mbr_size equ 0x200
 ;; MBR magic number
$mbr_magic equ 0xaa55
$alpha_addr equ 0x7c00

  ;; init stack accordingly
$bl_ss equ 0x0bc0
$bl_sp equ 0x0200

  ;; kernel loading addr
$kernel_offset equ 0x1000

[org $alpha_addr]

[bits 16]

_rm_start:
  ;; set ss, sp
  mov [boot_drive], dl
  mov dx, $bl_ss
  mov ss, dx
  mov sp, $bl_sp

  ;; clear screen
  call _rm_reset_screen
  call _rm_load_kernel
  call _rm_switch_pm ; never returns
  jmp $

  ;; read dh sectors from dl drive into es:bx
_rm_disk_load:
  pusha
  push dx

  mov ah, 0x02 ; ah <- int 0x13 function. 0x02 = 'read'
  mov al, dh   ; al <- number of sectors to read (0x01 .. 0x80)
  mov cl, 0x02 ; cl <- sector (0x01 .. 0x11)
               ; 0x01 is our boot sector, 0x02 is the first 'available' sector
  mov ch, 0x00 ; ch <- cylinder (0x0 .. 0x3FF, upper 2 bits in 'cl')
  ; dl <- drive number. Our caller sets it as a parameter and gets it from BIOS
  ; (0 = floppy, 1 = floppy2, 0x80 = hdd, 0x81 = hdd2)
  mov dh, 0x00 ; dh <- head number (0x0 .. 0xF)

  ; [es:bx] <- pointer to buffer where the data will be stored
  ; caller sets it up for us, and it is actually the standard location for int 13h
  int 0x13      ; BIOS interrupt
  jc _disk_error ; if error (stored in the carry bit)

  pop dx
  cmp al, dh    ; BIOS also sets 'al' to the # of sectors read. Compare it.
  jne _sectors_error

  popa
  ret

_disk_error:
_sectors_error:
  jmp $

_rm_load_kernel:
  mov bx, $kernel_offset
  mov dh, 54
  mov dl, [boot_drive]
  call _rm_disk_load
  ret

_rm_switch_pm:
  ;; switch to protected-mode
  cli
  lgdt [_gdt_descriptor]
  mov eax, cr0
  or al, 1
  mov cr0, eax

  jmp CODE_SEG:_init_pm

  ;; real-mode bios int to reset gfx mode
_rm_reset_screen:
  pusha

  mov ah, 0x00
  mov al, 0x02
  int 0x10

  popa
  ret

[bits 32]
_init_pm:
  mov ax, DATA_SEG
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov ebp, 0x40000
  mov esp, ebp

  call $kernel_offset

  jmp $

  ;; data
%include "boot/gdt.asm"

boot_drive db 0

  ;; make boot loader exactly 512 bytes long
times ($mbr_size-0x02)-($-$$) db 0x00
dw $mbr_magic
