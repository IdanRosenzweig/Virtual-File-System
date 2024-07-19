#ifndef SOFTLINK_H
#define SOFTLINK_H

#include <vector>
#include <string>

#include "../base_comp.h"
#include "src/hierarchy/path/path.h"

// a node that points to another path
struct softlink : public base_comp {
    path path_ptr;

    softlink() : base_comp(), path_ptr() {
    }

    explicit softlink(const path &path_ptr)
        : path_ptr(path_ptr) {
    }

    softlink(const softlink &other)
        : base_comp(other),
          path_ptr(other.path_ptr) {
    }

    softlink(softlink &&other) noexcept
        : base_comp(std::move(other)),
          path_ptr(std::move(other.path_ptr)) {
    }

    softlink &operator=(const softlink &other) {
        if (this == &other)
            return *this;
        base_comp::operator =(other);
        path_ptr = other.path_ptr;
        return *this;
    }

    softlink &operator=(softlink &&other) noexcept {
        if (this == &other)
            return *this;
        base_comp::operator =(std::move(other));
        path_ptr = std::move(other.path_ptr);
        return *this;
    }

};


#endif //SOFTLINK_H
