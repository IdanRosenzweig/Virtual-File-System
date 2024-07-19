#ifndef NULL_CONTENT_H
#define NULL_CONTENT_H

#include <utility>

#include "../base_content.h"

struct null_content : public base_content {

    null_content() : base_content() {}

    explicit null_content(content_id_t id)
        : base_content(id) {
    }

    null_content(const null_content &other)
        : base_content(other) {
    }

    null_content(null_content &&other) noexcept
        : base_content(std::move(other)) {
    }

    null_content & operator=(const null_content &other) {
        if (this == &other)
            return *this;
        base_content::operator =(other);
        return *this;
    }

    null_content & operator=(null_content &&other) noexcept {
        if (this == &other)
            return *this;
        base_content::operator =(std::move(other));
        return *this;
    }
};
#endif //NULL_CONTENT_H
