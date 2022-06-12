#pragma once

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char *str);
float truncf(float x);
void *memcpy(void *dest, const void * src, size_t n);
void *memset(void *str, int c, size_t n);
