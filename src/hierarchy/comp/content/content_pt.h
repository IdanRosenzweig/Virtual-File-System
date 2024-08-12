#ifndef CONTENT_PT_H
#define CONTENT_PT_H

#include "../base_comp.h"
#include "src/hierarchy/content/content_id.h"

struct content_pt : public base_comp {
    content_id_t ptr = content_id_null;

    content_pt() = default;

    explicit content_pt(const comp_id_t &id)
        : base_comp(id) {
    }

    content_pt(const content_pt &other)
        : base_comp(other),
          ptr(other.ptr) {
    }

    content_pt(content_pt &&other) noexcept
        : base_comp(move(other)),
          ptr(move(other.ptr)) {
    }

    content_pt &operator=(const content_pt &other) {
        if (this == &other)
            return *this;
        base_comp::operator =(other);
        ptr = other.ptr;
        return *this;
    }

    content_pt &operator=(content_pt &&other) noexcept {
        if (this == &other)
            return *this;
        base_comp::operator =(move(other));
        ptr = move(other.ptr);
        return *this;
    }
};
#endif //CONTENT_PT_H
