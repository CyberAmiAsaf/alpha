%define $real_mode 16         ;; real mode no. bits
%define $protected_mode 32    ;; protected no. mode bits

$mbr_size equ 0x200         ;; =512
$mbr_magic equ 0xaa55       ;; MBR magic number

$alpha_addr equ 0x7c00

$bl_ss equ 0x0bc0
$bl_sp equ 0x0200

$video_memory equ 0xb8000   ;; start of vga
$white_on_black equ 0x0f    ;; vga color

[org $alpha_addr]

[bits 16]

_rm_start:
  ;; set ss, sp
  mov dx, $bl_ss
  mov ss, dx
  mov sp, $bl_sp

  ;; clear screen
  call _rm_reset_screen

  ;; real-mode message
  mov bx, REAL_MODE_MSG
  call _rm_print

  ;; protected-mode switch
  jmp _rm_switch_pm

  jmp $

  ;; real-mode bios int to reset gfx mode
_rm_reset_screen:
  pusha

  mov ah, 0x00
  mov al, 0x02
  int 0x10

  popa
  ret

  ;; real-mode bios print string located @ BX
_rm_print:
  pusha

  mov ah, 0x0e

_rm_print_loop:
  mov al, [bx]
  test al, al
  jz _rm_print_ret

  int 0x10

  inc bx
  jmp _rm_print_loop

_rm_print_ret:
  popa
  ret

_rm_switch_pm:
  ;; turn off interrupts
  cli

  lgdt [gdt_descriptor]

  mov eax, cr0
  or eax, 0x01
  mov cr0, eax

  jmp CODE_SEG:_pm_init

[bits 32]

_pm_init:
  mov ax, DATA_SEG
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ebp, 0x90000
  mov esp, ebp

_pm_start:
  mov ebx, PROTECTED_MODE_MSG
  call _pm_print

  jmp $

  ;; protected-mode vga print string located @ EBX
_pm_print:
  pusha
  mov edx, $video_memory
  mov ah, $white_on_black

_pm_print_loop:
  mov al, [ebx]
  test al, al
  jz _pm_print_ret

  mov [edx], ax
  inc ebx
  add edx, 2
  jmp _pm_print_loop

_pm_print_ret:
  popa
  ret

;; data

%include "boot/gdt.asm"

REAL_MODE_MSG:
  db "16-bits real-mode initiated.", 0x0a, 0x0d, 0

PROTECTED_MODE_MSG:
  db "32-bits protected-mode initiated.", 0

  ;; make loader 512 bytes long
times ($mbr_size-0x02)-($-$$) db 0x00
dw $mbr_magic
