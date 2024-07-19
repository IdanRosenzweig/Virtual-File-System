#ifndef BASE_COMP_H
#define BASE_COMP_H

#include "comp_id.h"

struct basic_hierarchy;

struct base_comp {
    comp_id_t id = comp_id_null;

    base_comp() = default;

    explicit base_comp(comp_id_t id): id(id) {
    }

    virtual ~base_comp() {
    }

    base_comp(const base_comp &other)
        : id(other.id) {
    }

    base_comp(base_comp &&other) noexcept
        : id(other.id) {
        other.id = comp_id_null;
    }

    base_comp &operator=(const base_comp &other) {
        if (this == &other)
            return *this;
        id = other.id;
        return *this;
    }

    base_comp &operator=(base_comp &&other) noexcept {
        if (this == &other)
            return *this;
        id = other.id;
        other.id = comp_id_null;
        return *this;
    }
};

#endif //BASE_COMP_H
