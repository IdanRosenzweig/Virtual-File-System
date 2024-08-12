#include "random_stream_dev.h"
#include "lib/utility.h"

void random_stream_dev::read(void *buff, int cnt) {
    byte *dest = (byte *) buff;
    for (int i = 0; i < cnt; i++, dest++)
        *dest = (byte) 'a';
}
