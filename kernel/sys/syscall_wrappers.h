#pragma once

#include "../util.h"
#include "syscall_numbers.h"

int32_t write(int32_t fd, const void *buf, const uint32_t len)
{
    int32_t result = -1;

    __asm__ __volatile__ ("int $0x80" : "=a"(result) : "a"(SYSCALL_WRITE), "b"(fd), "c"(buf), "d"(len) );

    return result;
}
