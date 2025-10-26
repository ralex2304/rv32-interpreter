#pragma once

#include "api.h"

inline void* memcpy(void* dest, const void* src, size_t n) {
    while (n--)
        *((char*)dest) = *((char*)src);

    return dest;
}

inline int isspace(int c) {
    return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v';
}

inline int atoi(const char* str) {
    while (isspace(*str))
        ++str;

    bool negative = false;

    if (*str == '+') {
        ++str;
    } else if (*str == '-' ) {
        ++str;
        negative = true;
    }

    int result = 0;
    for (; '0' <= *str && *str <= '9'; ++str)
    {
        int digit = *str - '0';
        result *= 10;
        result -= digit; // calculate in negatives to support INT_MIN, LONG_MIN,..
    }

    return negative ? result : -result;
}

inline int print_int(const int a, int fd = 2) {
    static const int BUF_SIZE = 64;
    char buf[BUF_SIZE];

    int num = (a < 0) ? -a : a;

    buf[BUF_SIZE - 1] = '\n';
    int i = BUF_SIZE - 2;
    while (i >= 0 && num != 0) {
        buf[i--] = '0' + (num % 10);
        num /= 10;
    }

    if (num == 0 && i == BUF_SIZE - 2)
        return write(fd, "0", 1);

    if (a < 0 && i >= 0)
        buf[i--] = '-';

    return write(fd, buf + i + 1, BUF_SIZE - (i + 1));
}

