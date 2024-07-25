#include "textfile.h"


void read(open_file &file, uint8_t *dest, int cnt) {
    for (int i = 0; i < cnt; i++)
        dest[i] = file.file->text[file.ptr_off++];
}

void write(open_file &file, uint8_t *src, int cnt) {
    for (int i = 0; i < cnt; i++)
        file.file->text[file.ptr_off++] = src[i];
}

void seek(open_file &file, int off) {
    file.ptr_off = off;
}
