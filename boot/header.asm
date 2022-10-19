; ========== Multiboot2 Header ========== ;
; Handles some header information for multiboot2
; Finally adds all the information up into a checksum


section .multiboot_header
header_start:
    ; Magic number that multiboot2 will look for
    dd 0xe85250d6 ; Multiboot2
    ; Information on the architecture of the operating system
    dd 0 ; Protected mode i386
    ; Length of the header
    dd header_end - header_start
    ; Peform a checksum
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    ; Add in a end tag
    dw 0
    dw 0
    dd 8
header_end:
