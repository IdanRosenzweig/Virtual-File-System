#ifndef BASE_CONTENT_H
#define BASE_CONTENT_H

#include "content_id.h"

struct base_content {
    content_id_t id = content_id_null;

    base_content() = default;

    explicit base_content(content_id_t id): id(id) {
    }

    virtual ~base_content() {
    }

    base_content(const base_content &other)
        : id(other.id) {
    }

    base_content(base_content &&other) noexcept
        : id(other.id) {
    }

    base_content &operator=(const base_content &other) {
        if (this == &other)
            return *this;
        id = other.id;
        return *this;
    }

    base_content &operator=(base_content &&other) noexcept {
        if (this == &other)
            return *this;
        id = other.id;
        return *this;
    }
};

#endif //BASE_CONTENT_H
