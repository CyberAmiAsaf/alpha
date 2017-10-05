%define $real_mode 16           ;; real mode no. bits
%define $protected_mode 32      ;; protected no. mode bits

%define $mbr_size 0x200         ;; =512
%define $mbr_magic 0xaa55       ;; MBR magic number

%define $alpha_addr 0x7c00

%define $bl_ss 0x0bc0
%define $bl_sp 0x0200
