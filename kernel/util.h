#pragma once

typedef unsigned long long size_t;

typedef unsigned char uint8_t;
typedef unsigned char uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;

typedef enum {
    false,  // 0
    true    // 1
} bool; 

void print_fileTable();
void print_registers();
bool isspace(const char c);
