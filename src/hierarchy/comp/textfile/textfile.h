#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <cstdint>
#include <string.h>
#include <utility>

#include "../base_comp.h"

struct textfile : public base_comp {
#define TEXTFILE_MAX_SZ 600
    uint8_t text[TEXTFILE_MAX_SZ] = {0};

    textfile() : base_comp() {}

    textfile(const textfile &other)
        : base_comp(other) {
        memcpy(text, other.text, TEXTFILE_MAX_SZ);
    }

    textfile(textfile &&other) noexcept
        : base_comp(std::move(other)) {
        memcpy(text, other.text, TEXTFILE_MAX_SZ);
        memset(other.text, 0, TEXTFILE_MAX_SZ);
    }

    textfile & operator=(const textfile &other) {
        if (this == &other)
            return *this;
        base_comp::operator =(other);
        memcpy(text, other.text, TEXTFILE_MAX_SZ);
        return *this;
    }

    textfile & operator=(textfile &&other) noexcept {
        if (this == &other)
            return *this;
        base_comp::operator =(std::move(other));
        memcpy(text, other.text, TEXTFILE_MAX_SZ);
        memset(other.text, 0, TEXTFILE_MAX_SZ);
        return *this;
    }
};

struct open_file {
    textfile* file;
    int ptr_off;
};

void read(open_file& file, uint8_t* dest, int cnt);

void write(open_file& file, uint8_t* src, int cnt);

void seek(open_file& file, int off);

#endif //TEXTFILE_H
