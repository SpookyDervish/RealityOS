#pragma once

#include "../util.h"

enum {
    PRINT_COLOUR_BLACK = 0,
	PRINT_COLOUR_BLUE = 1,
	PRINT_COLOUR_GREEN = 2,
	PRINT_COLOUR_CYAN = 3,
	PRINT_COLOUR_RED = 4,
	PRINT_COLOUR_MAGENTA = 5,
	PRINT_COLOUR_BROWN = 6,
	PRINT_COLOUR_LIGHT_GRAY = 7,
	PRINT_COLOUR_DARK_GRAY = 8,
	PRINT_COLOUR_LIGHT_BLUE = 9,
	PRINT_COLOUR_LIGHT_GREEN = 10,
	PRINT_COLOUR_LIGHT_CYAN = 11,
	PRINT_COLOUR_LIGHT_RED = 12,
	PRINT_COLOUR_PINK = 13,
	PRINT_COLOUR_YELLOW = 14,
	PRINT_COLOUR_WHITE = 15,
};

void print_clear();
void print_char(char character);
void print_str(char* str);
void print_set_colour(uint8_t foreground, uint8_t background);
void print_backspace();