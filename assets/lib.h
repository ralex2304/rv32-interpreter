#pragma once

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

