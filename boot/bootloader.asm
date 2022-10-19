; ========== RealityOS Bootloader ========== ;
; The main bootloader for RealityOS
; Handles register setup, long mode, and other things


global start
extern long_mode_start

section .text
bits 32
start:
    ; The CPU uses the esp to determine the address of the current
    ; stack frame

    ; This is refered to as the stack pointer
    mov esp, stack_top

    ; Setup CPU and multiboot
    call check_multiboot
    call check_cpuid
    call check_long_mode

    ; Setup paging
    call setup_page_tables
    call enable_paging

    ; Load the global descriptor table
    lgdt [gdt64.pointer]
    jmp gdt64.code_segment:long_mode_start

check_multiboot:
    cmp eax, 0x36d76289 ; Magic number multiboot bootloaders leave in the eax register
    jne .no_multiboot
    ret
.no_multiboot:
    ; Display an error message if their CPU does nt support multiboot
    mov al, "M"
    jmp error

check_cpuid:
    ; Attemp to flip the ID bit of the flags register
    ; If we can flip it cpuid is available
    pushfd
    pop eax
    mov ecx, eax
    xor eax, 1 << 21
    push eax
    popfd
    pushfd
    pop eax
    push ecx
    popfd
    cmp eax, ecx
    je .no_cpuid ; cpuid is not supported
    ret
.no_cpuid:
    mov al, "C"
    jmp error

check_long_mode:
    ; Check if cpuid supports extended processor info
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb .no_long_mode ; If eax is less than this value is less than this value then extended processor info is not supported
                     ; and in turn long mode is not supported

    mov eax, 0x80000001
    cpuid
    ; cpuid will store a value into the edx register
    ; If the lm bit is set, long mode is available
    test edx, 1 << 29
    jz .no_long_mode ; Long mode is not available

    ret
.no_long_mode:
    mov al, "L"
    jmp error

setup_page_tables:
    mov eax, page_table_l3
    ; Enable some flags for the level 3 page table
    or eax, 0b11 ; Present, writable
    mov [page_table_l4], eax

    mov eax, page_table_l2
    ; Enable some flags for the level 2 page table
    or eax, 0b11 ; Present, writable
    mov [page_table_l3], eax

    ; Fill up all the entries of the level 2 page table
    mov ecx, 0 ; Counter
.loop:
    mov eax, 0x200000 ; 2MiB
    mul ecx
    or eax, 0b10000011 ; Present, writable, huge page
    mov [page_table_l2 + ecx * 8], eax

    inc ecx ; Increment the counter
    cmp ecx, 512 ; Checks if the whole table is mapped
    jne .loop ; If not, continue looping

    ret

enable_paging:
    ; Pass the page table location to the CPU
    mov eax, page_table_l4
    mov cr3, eax

    ; Enable PAE so that 64-bit paging is available
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Finally! Enable long mode!
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8 ; Enable long mode flag
    wrmsr

    ; Enable paging
    mov eax, cr0
    or eax, 1 << 31 ; Enable paging bit
    mov cr0, eax

    ret

error:
    ; Print "ERR: X" where X is the error code
    mov dword [0xb8000], 0x4f52f445
    mov dword [0xb8000], 0x4f3a4f52
    mov dword [0xb8000], 0x4f204f20
    mov byte  [0xb8000], al
    hlt ; Halt the CPU

section .bss
align 4096
page_table_l4:
    resb 4096
page_table_l3:
    resb 4096
page_table_l2:
    resb 4096
stack_bottom:
    ; Reserve 16Kb of memory for loading kernel
    resb 4096 * 4
stack_top:

section .rodata
gdt64:
    dq 0 ; Zero entry
.code_segment: equ $ - gdt64
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53) ; Code segment
.pointer:
    dw $ - gdt64 - 1
    dq gdt64
