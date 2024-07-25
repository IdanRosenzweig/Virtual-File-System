#ifndef MOUNT_H
#define MOUNT_H

#include <utility>

#include "../base_comp.h"

struct hierarchy;

struct mount : public base_comp {
    hierarchy* target_hier = nullptr;

    mount() : base_comp() {
    }

    explicit mount(hierarchy *mnt_fs)
        : base_comp(), target_hier(mnt_fs) {
    }

    mount(const mount &other)
        : base_comp(other),
          target_hier(other.target_hier) {
    }

    mount(mount &&other) noexcept
        : base_comp(std::move(other)),
          target_hier(other.target_hier) {
    }

    mount & operator=(const mount &other) {
        if (this == &other)
            return *this;
        base_comp::operator =(other);
        target_hier = other.target_hier;
        return *this;
    }

    mount & operator=(mount &&other) noexcept {
        if (this == &other)
            return *this;
        base_comp::operator =(std::move(other));
        target_hier = other.target_hier;
        return *this;
    }
};
#endif //MOUNT_H
