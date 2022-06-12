#include "mini_libc.h"

size_t strlen(const char *str) {
    size_t len = 0;
    while (*str != 0) {
        len++;
        str++;
    }
    return len;
}

#define FORCE_EVAL(x) ((void)(x))

float truncf(float x)
{
	union {float f; uint32_t i;} u = {x};
	int e = (int)(u.i >> 23 & 0xff) - 0x7f + 9;
	uint32_t m;

	if (e >= 23 + 9)
		return x;
	if (e < 9)
		e = 1;
	m = -1U >> e;
	if ((u.i & m) == 0)
		return x;
	FORCE_EVAL(x + 0x1p120f);
	u.i &= ~m;
	return u.f;
}

void *memcpy(void *dest, const void * src, size_t n) {
    char *d = dest;
    const char *s = src;
    for (size_t i = 0; i < n; i++)
    {
        *d = *s;
        d++;
        s++;
    }
    return d;
}
void *memset(void *str, int c, size_t n) {
    char *d = str;
    char s = c;
    for (size_t i = 0; i < n; i++) {
        *d = s;
        d++;
    }
    return d;
}
