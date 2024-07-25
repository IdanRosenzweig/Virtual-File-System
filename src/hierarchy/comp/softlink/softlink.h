#ifndef SOFTLINK_H
#define SOFTLINK_H

#include <vector>

#include "../base_comp.h"
#include "src/hierarchy/ctx_t.h"
#include "src/hierarchy/path/path.h"

// a node that points to another path
struct softlink : public base_comp {
    ctx_t<path> target;

    softlink() : base_comp(), target() {
    }

    explicit softlink(const ctx_t<path> &target)
        : target(target) {
    }

    softlink(const softlink &other)
        : base_comp(other),
          target(other.target) {
    }

    softlink(softlink &&other) noexcept
        : base_comp(std::move(other)),
          target(std::move(other.target)) {
    }

    softlink & operator=(const softlink &other) {
        if (this == &other)
            return *this;
        base_comp::operator =(other);
        target = other.target;
        return *this;
    }

    softlink & operator=(softlink &&other) noexcept {
        if (this == &other)
            return *this;
        base_comp::operator =(std::move(other));
        target = std::move(other.target);
        return *this;
    }
};


#endif //SOFTLINK_H
