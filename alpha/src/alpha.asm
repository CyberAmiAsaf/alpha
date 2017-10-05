%include "./definitions.asm"

[bits $real_mode]
[org $alpha_addr]

_main:
  ;; set ss, sp
  mov dx, $bl_ss
  mov ss, dx
  mov sp, $bl_sp

  ;; set ds
  mov ax, $alpha_addr

  ;; clear screen
  call _clear_real

  ;; real-mode message
  mov bx, _welcome_real_mode
  call _print_real
  jmp $

%include "./print_real.asm"

_welcome_real_mode:
  db "Initiated 16-bits real-mode...", 0

_welcome_protected_mode:
  db "Converted to 32-bits protected-mode...", 0

  ;; make loader 512 bytes long
times ($mbr_size-0x02)-($-$$) db 0x00
dw $mbr_magic
