#include "random_stream_dev.h"

#include <cstdint>

void random_stream_dev::read(void *buff, int cnt) {
    uint8_t *dest = (uint8_t *) buff;
    for (int i = 0; i < cnt; i++, dest++)
        *dest = (uint8_t) 'a';
}
