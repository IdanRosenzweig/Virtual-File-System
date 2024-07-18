#ifndef TEXT_FILE_H
#define TEXT_FILE_H

#include <cstdint>
#include <string.h>
#include <utility>

#include "../base_data.h"

struct text_file : public base_data {
#define TEXT_FILE_MAX_SZ 600
    uint8_t text[TEXT_FILE_MAX_SZ] = {0};

    text_file() : base_data() {}

    text_file(const text_file &other)
        : base_data(other) {
        memcpy(text, other.text, TEXT_FILE_MAX_SZ);
    }

    text_file(text_file &&other) noexcept
        : base_data(std::move(other)) {
        memcpy(text, other.text, TEXT_FILE_MAX_SZ);
        memset(other.text, 0, TEXT_FILE_MAX_SZ);
    }

    text_file & operator=(const text_file &other) {
        if (this == &other)
            return *this;
        base_data::operator =(other);
        memcpy(text, other.text, TEXT_FILE_MAX_SZ);
        return *this;
    }

    text_file & operator=(text_file &&other) noexcept {
        if (this == &other)
            return *this;
        base_data::operator =(std::move(other));
        memcpy(text, other.text, TEXT_FILE_MAX_SZ);
        memset(other.text, 0, TEXT_FILE_MAX_SZ);
        return *this;
    }
};

struct open_file {
    text_file* file;
    int ptr_off;
};

void read(open_file& file, uint8_t* dest, int cnt);

void write(open_file& file, uint8_t* src, int cnt);

void seek(open_file& file, int off);

#endif //TEXT_FILE_H
