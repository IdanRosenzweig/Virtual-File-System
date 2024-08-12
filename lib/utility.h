#ifndef UTILITY_H
#define UTILITY_H

using byte = char;

void memcpy(void *dest, const void *src, int cnt);

void memset(void *dest, byte val, int cnt);

void strcpy(void* dest, const void* src);

#endif //UTILITY_H
