// ========== :: exceptions.h ::  ========== \\
// Used to create functions / ISRs for exceptions
// Division by zero, memory error, etc.


#pragma once

#include "idt.h"
#include "../drivers/screen.h"

__attribute__ ((interrupt)) void div_by_0_handler(int_frame_32_t *frame)
{
    print_set_colour(PRINT_COLOUR_LIGHT_RED, PRINT_COLOUR_BLACK);
    print_str("[ERROR] :: DIVIDE BY ZERO - EXCEPTION HANDLED");

    // Attemp to move on after the offending DIV / IDIV instruction by incrementing instruction pointer
    frame->eip++;
}
