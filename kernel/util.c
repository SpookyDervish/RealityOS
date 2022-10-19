#include "disk/file_ops.h"
#include "util.h"
#include "drivers/screen.h"
#include "libc/cstr.h"

void print_fileTable()
{
    uint8_t *fileTable_name = "fileTable ";
    uint8_t *filetable_ptr;
    uint16_t blanks_num = 0;
    uint8_t *file_ext = "   ";
    uint16_t hex_num = 0;
    uint8_t *heading = "\x0A\x0D"
        "---------   ---------   -------   ------------   --------------" "\n"
        "File Name   Extension   Entry #   Start Sector   Size (sectors)" "\n" 
        "---------   ---------   -------   ------------   --------------" "\n";

    // Print file table heading
    print_str(heading); 

    // Load file table string from its memory location (1000h), print file
    //   and program names & sector numbers to screen
    // --------------------------------------------------------------------
    // Load file table sector from disk to memory, to get any updates
    load_file(fileTable_name, (uint16_t)10, (uint32_t)0x1000, file_ext); 

    filetable_ptr = (uint8_t *)0x1000;

    // Filename loop (0 = end of filetable)
    while (*filetable_ptr != 0) {
        // Print file name
        for (uint8_t i = 0; i < 10; i++) {
            print_char(*filetable_ptr);
            filetable_ptr++;             // Get next byte at file table
        }

        // 2 blanks before file extension
        print_char(0x20);
        print_char(0x20);

        // Print file ext bytes
        for (uint8_t i = 0; i < 3; i++) {
            print_str(*filetable_ptr);
            filetable_ptr++;
        }

        // Directory entry # section
        // 9 blanks before entry #
        for (uint8_t i = 0; i < 9; i++)
            print_char(0x20);
        
        // Print hex value
        hex_num = *filetable_ptr;
        print_str(to_string(hex_num));
        filetable_ptr++;

        // Starting sector section
        // 4 blanks before starting sector
        for (uint8_t i = 0; i < 4; i++)
            print_char(0x20);
        
        // Print hex value
        hex_num = *filetable_ptr;
        print_str(to_string(hex_num));
        filetable_ptr++;

        // File size section
        // 9 blanks before file size
        for (uint8_t i = 0; i < 9; i++)
            print_char(0x20);
        
        // Print hex value
        hex_num = *filetable_ptr;
        print_str(to_string(hex_num));
        filetable_ptr++;

        // Print newline bewtween file table entries
        print_char(0x0A);
    }
}

void print_registers()
{
    uint32_t hex_num = 0;

    // TODO: Save all registers (& flags?) at start of file,
    //  then use them to print their original values before
    //  this file was called
    
    print_str("--------  ------------\n");
    print_str("Register  Mem Location\n");
    print_str("--------  ------------\n");

    // TODO: Change to print out DX after CX, so all are in order

	// Print string for DX
    __asm__ ("movl %%edx, %0" : "=r" (hex_num) );
    print_str("\nedx        ");
    print_str(to_string(hex_num));

	// Print string for AX
    __asm__ ("movl %%eax, %0" : "=r" (hex_num) );
    print_str("\neax        ");
    print_str(to_string(hex_num));

	// Print string for BX
    __asm__ ("movl %%ebx, %0" : "=r" (hex_num) );
    print_str("\nebx        ");
    print_str(to_string(hex_num));

	// Print string for CX
    __asm__ ("movl %%ecx, %0" : "=r" (hex_num) );
    print_str("\necx        ");
    print_str(to_string(hex_num));

	// Print string for SI
    __asm__ ("movl %%esi, %0" : "=r" (hex_num) );
    print_str("\nesi        ");
    print_str(to_string(hex_num));

	// Print string for DI
    __asm__ ("movl %%edi, %0" : "=r" (hex_num) );
    print_str("\nedi        ");
    print_str(to_string(hex_num));

	// Print string for CS
    __asm__ ("movl %%cs, %0" : "=r" (hex_num) );
    print_str("\ncs         ");
    print_str(to_string(hex_num));

	// Print string for DS
    __asm__ ("movl %%ds, %0" : "=r" (hex_num) );
    print_str("\nds         ");
    print_str(to_string(hex_num));

	// Print string for ES
    __asm__ ("movl %%es, %0" : "=r" (hex_num) );
    print_str("\nes         ");
    print_str(to_string(hex_num));

	// Print string for SS
    __asm__ ("movl %%ss, %0" : "=r" (hex_num) );
    print_str("\nss         ");
    print_str(to_string(hex_num));

	// Print newline when done
    print_str("\n");
}

// Is C a whitespace character?
bool isspace(const char c)
{
    return (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\v' ||
            c == '\b');
}
