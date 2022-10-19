#pragma once
#include "../util.h"

const char* to_string(uint64_t value);
const char* to_hstring(uint64_t value);
const char* to_dstring(double value, uint8_t decimalPlaces);
int16_t strcmp(const uint8_t *string1, const uint8_t *string2);
int16_t strncmp(const uint8_t *string1, const uint8_t *string2, uint8_t len);
uint8_t strlen(const uint8_t *string);
uint8_t *strcpy(uint8_t *dst, const uint8_t *src);
uint8_t *strncpy(uint8_t *dst, const uint8_t *src, const uint8_t len);
void *memset(void *buffer, const uint8_t byte, const uint32_t len);
void *memcpy(void *dst, const void *src, const uint32_t len);
void *memcpy32(void *dst, const void *src, const uint32_t len);
