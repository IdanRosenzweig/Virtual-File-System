#include "utility.h"

void memcpy(void *dest, const void *src, int cnt) {
    for (int i = 0; i < cnt; i++) ((byte *) dest)[i] = ((byte *) src)[i];
}

void memset(void *dest, byte val, int cnt) {
    for (int i = 0; i < cnt; i++) ((byte *) dest)[i] = val;
}

void strcpy(void *dest, const void *src) {
    int offset = 0;
    do {
        ((byte *) dest)[offset] = ((byte *) src)[offset];
    } while (((byte *) src)[offset++] != 0) ;
}
