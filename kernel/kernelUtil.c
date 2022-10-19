#include "kernelUtil.h"
#include "drivers/screen.h"

void panic(char* msg)
{
    print_set_colour(PRINT_COLOUR_LIGHT_RED, PRINT_COLOUR_BLACK);
    print_str(msg);

    while (1);
}
