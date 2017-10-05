[bits $real_mode]
[org $alpha_addr]
_clear_real:
  pusha

  mov ah, 0x00
  mov al, 0x02
  int 0x10
  
  popa
  ret

  ;; IN: BX=str addr
  ;; OUT: print str
_print_real:
  pusha

  mov ah, 0x0e

_print_real_loop:
  mov al, [bx]
  test al, al
  je _print_real_ret

  int 0x10

  inc bx
  jmp _print_real_loop

_print_real_ret:
  popa
  ret
