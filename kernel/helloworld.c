#include "drivers/screen.h"

__attribute__ ((section ("helloworld_entry"))) void calc_main(void)
{
    print_str("Hello World!");
}