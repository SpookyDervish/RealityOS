// ========== RealityOS Kernel ========== \\
// The main kernel file for RealityOS
// Initializes memory and sets up interupts

#include "./drivers/screen.h"
#include "kernelUtil.h"
#include "cpu/idt.h"
#include "cpu/exceptions.h"
#include "drivers/get_key.h"
#include "libc/cstr.h"
#include "memory/physical_memory_manager.h"
#include "memory/virtual_memory_manager.h"
#include "ports/io.h"

void print_physical_memory_info(void); // Print information from the physical memory map (SMAP)

void kernel_main()
{
    uint8_t *file_ptr;
    uint8_t *menuString = "----------------------------------------------------------------\n"
                          "Kernel Booted, Welcome to RealityOS - The best operating system!\n"
                          "----------------------------------------------------------------\n\n";
    uint8_t *failure = "\n"
                       "Command/Program not found, Try again"
                       "\n";
    uint8_t *prompt = ">:$ ";
    uint8_t *pgmNotLoaded = "\n"
                            "Program found but not loaded, Try Again"
                            "\n";
    uint8_t *fontNotFound = "\n"
                            "Font not found!"
                            "\n";

    char cmdString[256] = {0}; // User input string
    char *cmdString_ptr = cmdString;
    uint8_t input_char = 0;                // User input character
    uint8_t input_length;                  // Length of user input
    uint8_t *cmdDir = "dir\0";             // Directory command; list all files/pgms on disk
    uint8_t *cmdReboot = "reboot\0";       // 'warm' reboot option
    uint8_t *cmdPrtreg = "prtreg\0";       // Print register values
    uint8_t *cmdHlt = "hlt\0";             // E(n)d current program by halting cpu
    uint8_t *cmdCls = "cls\0";             // Clear screen by scrolling
    uint8_t *cmdDelFile  = "del\0";		   // Delete a file from disk
    uint8_t *cmdRenFile  = "ren\0";        // Rename a file in the file table
    uint8_t *cmdPrtmemmap = "prtmemmap\0"; // Print physical memory map info
    uint8_t *cmdInfo = "sysinfo\0";        // Displays system info
    uint8_t *cmdHelp = "help\0";           // Displays a help messgae and commands list

    char *versionStr = "1.0.0";

    uint32_t needed_blocks, needed_pages;

    int argc = 0;
    char *argv[10] = {0};

    print_clear();

    print_set_colour(PRINT_COLOUR_YELLOW, PRINT_COLOUR_BLACK);
    print_str(menuString);
    print_set_colour(PRINT_COLOUR_WHITE, PRINT_COLOUR_BLACK);

    // Set up exception handlers  (ISRs 0-31)
    set_idt_descriptor_32(0, div_by_0_handler, TRAP_GATE_FLAGS); // Divide by 0 error, ISR 0
    // Set up additional exceptions here...

    print_str("[BOOT] :: Kernel initialized successfully\n");
    print_str("Type ");
    print_set_colour(PRINT_COLOUR_LIGHT_GREEN, PRINT_COLOUR_BLACK);
    print_str("'help' ");
    print_set_colour(PRINT_COLOUR_WHITE, PRINT_COLOUR_BLACK);
    print_str("to see alist of commands\n");

    print_fileTable();
    print_str("\n");

    while (1)
    {
        // Print prompt
        print_str(prompt);

        input_length = 0; // reset byte counter of input

        while (1)
        {
            input_char = get_key();

            if (input_char == '\r')
            { // enter key?
                print_char('\n');
                break; // go on to tokenize user input line
            }

            // TODO: Move all input back 1 char/byte after backspace, if applicable
            if (input_char == '\b')
            { // backspace?
                if (input_length > 0)
                {                                   // Did user input anything?
                    input_length--;                 // yes, go back one char in cmdString
                    cmdString[input_length] = '\0'; // Blank out character
                }

                continue; // Get next character
            }

            cmdString[input_length] = input_char; // Store input char to string
            input_length++;                       // Increment byte counter of input

            // Print input character to screen
            print_char(input_char);
        }

        if (input_length == 0)
        {
            // No input or command not found! boo D:
            print_str(failure);

            continue;
        }

        cmdString[input_length] = '\0'; // else null terminate cmdString from si

        // Tokenize input string "cmdString" into separate tokens
        cmdString_ptr = cmdString; // Reset pointers...
        argc = 0;                  // Reset argument count
        memset(argv, 0, sizeof argv);

        // Get token loop
        while (*cmdString_ptr != '\0')
        {
            // Skip whitespace between tokens
            while (isspace(*cmdString_ptr))
                *cmdString_ptr++ = '\0';

            // Found next non space character, start of next input token/argument
            argv[argc++] = cmdString_ptr;

            // Go to next space or end of string
            while (!isspace(*cmdString_ptr) && *cmdString_ptr != '\0')
                cmdString_ptr++;
        }

        /*print_str(argv[0]);    // Debugging
        print_char(' ');
        print_str(cmdDir);
        print_char(' ');
        print_str(to_string((uint64_t)strlen(cmdDir)));
        print_char(' ');*/

        // Check commands
        // Get first token (command to run) & second token (if applicable e.g. file name)
        if (strncmp(argv[0], cmdDir, strlen(cmdDir)) == 0)
        {
            // --------------------------------------------------------------------
            // File/Program browser & loader
            // --------------------------------------------------------------------
            print_fileTable();
            continue;
        }

        if (strncmp(argv[0], cmdCls, strlen(cmdCls)) == 0)
        {
            // --------------------------------------------------------------------
            // Clear Screen
            // --------------------------------------------------------------------
            print_clear();

            continue;
        }

        if (strncmp(argv[0], cmdReboot, strlen(cmdReboot)) == 0)
        {
            // --------------------------------------------------------------------
            // Reboot: Reboot the PC
            // --------------------------------------------------------------------
            outb(0x64, 0xFE); // Send "Reset CPU" command to PS/2 keyboard controller port
            continue;
        }

        if (strncmp(argv[0], cmdPrtreg, strlen(cmdPrtreg)) == 0)
        {
            // --------------------------------------------------------------------
            // Print Register Values
            // --------------------------------------------------------------------
            print_registers();
            continue;
        }

        if (strncmp(argv[0], cmdHlt, strlen(cmdHlt)) == 0)
        {
            // --------------------------------------------------------------------
            // End Program
            // --------------------------------------------------------------------
            __asm__("cli;hlt"); // Clear interrupts & Halt cpu
        }

        if (strncmp(argv[0], cmdCls, strlen(cmdCls)) == 0)
        {
            // --------------------------------------------------------------------
            // Clear Screen
            // --------------------------------------------------------------------
            print_clear();

            continue;
        }

        // Print memory map command
        if (strncmp(argv[0], cmdPrtmemmap, strlen(cmdPrtmemmap)) == 0)
        {
            // Print out physical memory map info
            print_str("\n-------------------\nPhysical Memory Map"
                      "\n-------------------\n\n");
            print_physical_memory_info();
            continue;
        }

        if (strncmp(argv[0], cmdInfo, strlen(cmdInfo)) == 0)
        {
            // --------------------------------------------------------------------
            // Info Command
            // --------------------------------------------------------------------
            print_str("\n-----------\n");
            print_str("System Info\n");
            print_str("-----------\n");

            print_str("Thank ");
            print_set_colour(PRINT_COLOUR_YELLOW, PRINT_COLOUR_BLACK);
            print_str("You ");
            print_set_colour(PRINT_COLOUR_WHITE, PRINT_COLOUR_BLACK);
            print_str("for using RealityOS!\n");
            print_str("RealityOS Version: ");
            print_str(versionStr);
            print_str("\n\n");

            continue;
        }

        if (strncmp(argv[0], cmdHelp, strlen(cmdHelp)) == 0)
        {
            // --------------------------------------------------------------------
            // Help Command
            // --------------------------------------------------------------------
            print_str("\n-----------------------------------\n");
            print_str("Command    Category     Description\n");
            print_str("-----------------------------------\n");
            print_str("dir        files        Directory command; list all files/pgms on disk\n");
            print_str("reboot     system       'warm' reboot option\n");
            print_str("prtreg     diagnostics  Print register values\n");
            print_str("hlt        computer     E(n)d current program by halting cpu\n");
            print_str("cls        screen       Clear screen by scrolling\n");
            print_str("prtmemmap  system       Print physical memory map info\n");
            print_str("sysinfo    diagnostics  Displays system info\n");
            print_str("help       help         Displays a help messgae and commands list\n");
            print_str("del        files        Delete a file from disk\n");
            print_str("ren        files        Rename a file in the file table\n");

            print_char("\n");

            continue;
        }

        if (strncmp(argv[0], cmdDelFile, strlen(cmdDelFile)) == 0) {
            // TODO: Fix deleting erasing filetable from "dir" command until reboot
            // --------------------------------------------------------------------
            // Delete a file from the disk
            // --------------------------------------------------------------------
            //	Input 1 - File name to delete
            //	      2 - Length of file name
            if (!delete_file(argv[1], strlen(argv[1]))) {
                //	;; TODO: Add error message or code here
                print_set_colour(PRINT_COLOUR_LIGHT_RED, PRINT_COLOUR_BLACK);
                print_str("ERROR: Delete file failed!");
                print_set_colour(PRINT_COLOUR_WHITE, PRINT_COLOUR_BLACK);
            }

            // Print newline when done
            print_str("\n\n");

            continue;
        }

        if (strncmp(argv[0], cmdRenFile, strlen(cmdRenFile)) == 0) {
            // --------------------------------------------------------------------
            // Rename a file in the file table
            // --------------------------------------------------------------------
            // 1 - File to rename
            // 2 - Length of name to rename
            // 3 - New file name // 4 - New file name length
            if (rename_file(argv[1], strlen(argv[1]), argv[2], strlen(argv[2])) != 0) {
                //	;; TODO: Add error message or code here
            }

            // Print newline when done
            print_char("\n");

            continue;
        }

        // If command not input, search file table entries for user input file
        file_ptr = check_filename(argv[0], strlen(argv[0]));
        if (*file_ptr == 0)
        {
            print_str(failure); // File not found in filetable, error

            continue;
        }

        // file_ptr is pointing to filetable entry, get number of pages needed to load the file
        //   num_pages = (file size in sectors * # of bytes in a sector) / size of a page in bytes
        needed_pages = (file_ptr[15] * 512) / PAGE_SIZE;  // Convert file size in bytes to pages
        if ((file_ptr[15] * 512) % PAGE_SIZE > 0) needed_pages++;   // Allocate extra page for partial page of memory

        print_str("\nAllocating ");
        print_set_colour(PRINT_COLOUR_LIGHT_CYAN, PRINT_COLOUR_BLACK);
        print_str(to_string(needed_pages));
        print_set_colour(PRINT_COLOUR_WHITE, PRINT_COLOUR_BLACK);
        print_str(" page(s)\n");

        // Load files/programs to this starting address
        const uint32_t entry_point = 0x400000;  // Put entry point right after identity mapped 4MB page table
    }
}

// Print information from the physical memory map (SMAP)
void print_physical_memory_info(void)
{
    // Physical memory map entry from Bios Int 15h EAX E820h
    typedef struct SMAP_entry
    {
        uint64_t base_address;
        uint64_t length;
        uint32_t type;
        uint32_t acpi;
    } __attribute__((packed)) SMAP_entry_t;

    uint32_t num_entries = *(uint32_t *)SMAP_NUMBER_ADDRESS;         // Number of SMAP entries
    SMAP_entry_t *SMAP_entry = (SMAP_entry_t *)SMAP_ENTRIES_ADDRESS; // Memory map entries start point

    for (uint32_t i = 0; i < num_entries; i++)
    {
        print_str("Region: ");
        print_str(to_string(i));
        print_str(" base: ");
        print_str(to_string(SMAP_entry->base_address));
        print_str(" length: ");
        print_str(to_string(SMAP_entry->length));
        print_str(" type: ");
        print_str(to_string(SMAP_entry->type));

        switch (SMAP_entry->type)
        {
        case 1:
            print_str(" (Available)");
            break;
        case 2:
            print_str(" (Reserved)");
            break;
        case 3:
            print_str(" (ACPI Reclaim)");
            break;
        case 4:
            print_str(" (ACPI NVS Memory)");
            break;
        default:
            print_str(" (Reserved)");
            break;
        }

        print_str("\r\n");
        SMAP_entry++; // Go to next entry
    }

    // Print total amount of memory
    SMAP_entry--; // Get last SMAP entry
    print_str("\r\nTotal memory in bytes: ");
    print_str(to_string(SMAP_entry->base_address + SMAP_entry->length - 1));

    // Print out memory manager block info:
    //   total memory in 4KB blocks, total # of used blocks, total # of free blocks
    print_str("\r\nTotal 4KB blocks: ");
    print_str(to_string(max_blocks));
    print_str("\r\nUsed or reserved blocks: ");
    print_str(to_string(used_blocks));
    print_str("\r\nFree or available blocks: ");
    print_str(to_string(max_blocks - used_blocks));
    print_str("\r\n\r\n");
}
