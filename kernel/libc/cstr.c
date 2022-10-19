#include "cstr.h"
#include "../util.h"

char uintTo_StringOutput[128];
const char* to_string(uint64_t value){
    uint8_t size;
    uint64_t sizeTest = value;
    while (sizeTest / 10 > 0){
        sizeTest /= 10;
        size++;
    }

    uint8_t index = 0;
    while(value / 10 > 0){
        uint8_t remainder = value % 10;
        value /= 10;
        uintTo_StringOutput[size - index] = remainder + '0';
        index++;
    }
    uint8_t remainder = value % 10;
    uintTo_StringOutput[size - index] = remainder + '0';
    uintTo_StringOutput[size + 1] = 0; 
    return uintTo_StringOutput;
}

char hexTo_StringOutput[128];
const char* to_hstring(uint64_t value){
    uint64_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 8 * 2 - 1;
    for (uint8_t i = 0; i < size; i++){
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    hexTo_StringOutput[size + 1] = 0;
    return hexTo_StringOutput;
}

/*char doubleTo_StringOutput[128];
const char* to_dstring(double value, uint8_t decimalPlaces){
    if (decimalPlaces > 20) decimalPlaces = 20;

    char* intPtr = (char*)to_string((int64_t)value);
    char* doublePtr = doubleTo_StringOutput;

    if (value < 0){
        value *= -1;
    }

    while(*intPtr != 0){
        *doublePtr = *intPtr;
        intPtr++;
        doublePtr++;
    }

    *doublePtr = '.';
    doublePtr++;

    double newValue = value - (int)value;

    for (uint8_t i = 0; i < decimalPlaces; i++){
        newValue *= 10;
        *doublePtr = (int)newValue + '0';
        newValue -= (int)newValue;
        doublePtr++;
    }

    *doublePtr = 0;
    return doubleTo_StringOutput;
}*/

// strcmp: Compare NUL-terminated string1 to string2
//
// returns:
//   negative value if string1 is less than string2
//   0 if string1 = string2
//   positive value if string1 is greater than string2
int16_t strcmp(const uint8_t *string1, const uint8_t *string2)
{
    while (*string1 && *string1 == *string2) {
        string1++;
        string2++;
    }

    return *string1 - *string2;
}

// strncmp: Compare NUL-terminated string1 to string2, at most len characters
//
// returns:
//   negative value if string1 is less than string2
//   0 if string1 = string2
//   positive value if string1 is greater than string2
int16_t strncmp(const uint8_t *string1, const uint8_t *string2, uint8_t len)
{
    while (len > 0  && *string1 == *string2) {
        string1++;
        string2++;
        len--;
    }
    if (len == 0) return 0;
    else          return *string1 - *string2;
}

// strlen: Get length of a string up until the NUL terminator
//
// Returns:
//   length (255 max)
uint8_t strlen(const uint8_t *string)
{
    uint8_t len = 0;

    while (*string) {
        string++;
        len++;
    }

    return len;
}

// strcpy: Copy src string to dst string (assuming src is <= dst!)
//
// Returns:
//   dst string 
uint8_t *strcpy(uint8_t *dst, const uint8_t *src)
{
    for (uint8_t i = 0; src[i]; i++)
        dst[i] = src[i];

    return dst;
}

// strncpy: Copy at most len characters from src string to dst string
//
// Returns:
//   dst string 
uint8_t *strncpy(uint8_t *dst, const uint8_t *src, const uint8_t len)
{
    for (uint8_t i = 0; src[i] && i < len; i++)
        dst[i] = src[i];

    return dst;
}

// memset: Write len # of bytes to a buffer
//
// Returns:
//   buffer
void *memset(void *buffer, const uint8_t byte, const uint32_t len)
{
    uint8_t *ptr = (uint8_t *)buffer;

    for (uint32_t i = 0; i < len; i++)
        ptr[i] = byte;

    return buffer;
}

// memcpy: Copy len bytes from src buffer to dst buffer
//
// Returns:
//  buffer
void *memcpy(void *dst, const void *src, const uint32_t len)
{
    for (uint32_t i = 0; i < len; i++)
        ((uint8_t *)dst)[i] = ((uint8_t *)src)[i];

    return dst;
}

// memcpy32: Copy len bytes from src buffer to dst buffer, 4 bytes at a time
//
// Returns:
//  buffer
void *memcpy32(void *dst, const void *src, const uint32_t len)
{
    for (uint32_t i = 0; i < len/4; i++)
        ((uint32_t *)dst)[i] = ((uint32_t *)src)[i];

    return dst;
}