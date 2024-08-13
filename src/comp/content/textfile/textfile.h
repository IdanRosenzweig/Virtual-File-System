#ifndef TEXTFILE_H
#define TEXTFILE_H

#include "lib/utility.h"

#include "../base_content.h"

struct textfile : public base_content {
#define TEXTFILE_MAX_SZ 600
    byte text[TEXTFILE_MAX_SZ] = {0};

    textfile() : base_content() {}

    explicit textfile(const content_id_t &id)
        : base_content(id) {
    }

    textfile(const textfile &other)
        : base_content(other) {
        memcpy(text, other.text, TEXTFILE_MAX_SZ);
    }

    textfile(textfile &&other) noexcept
        : base_content(move(other)) {
        memcpy(text, other.text, TEXTFILE_MAX_SZ);
        memset(other.text, 0, TEXTFILE_MAX_SZ);
    }

    textfile & operator=(const textfile &other) {
        if (this == &other)
            return *this;
        base_content::operator =(other);
        memcpy(text, other.text, TEXTFILE_MAX_SZ);
        return *this;
    }

    textfile & operator=(textfile &&other) noexcept {
        if (this == &other)
            return *this;
        base_content::operator =(move(other));
        memcpy(text, other.text, TEXTFILE_MAX_SZ);
        memset(other.text, 0, TEXTFILE_MAX_SZ);
        return *this;
    }
};

struct open_file {
    textfile* file;
    int ptr_off;
};

void read(open_file& file, byte* dest, int cnt);

void write(open_file& file, byte* src, int cnt);

void seek(open_file& file, int off);

#endif //TEXTFILE_H
