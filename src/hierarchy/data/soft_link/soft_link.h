#ifndef SOFT_LINK_H
#define SOFT_LINK_H

#include <vector>
#include <string>

#include "../base_data.h"
#include "src/hierarchy/path/path.h"
#include "src/hierarchy/data/node_data_id.h"

// a node that points to another path
struct soft_link : public base_data {
    path path_ptr;

    soft_link() : path_ptr() {
    }

    soft_link(const soft_link &other)
        : base_data(other),
          path_ptr(other.path_ptr) {
    }

    soft_link(soft_link &&other) noexcept
        : base_data(std::move(other)),
          path_ptr(std::move(other.path_ptr)) {
    }

    soft_link &operator=(const soft_link &other) {
        if (this == &other)
            return *this;
        base_data::operator =(other);
        path_ptr = other.path_ptr;
        return *this;
    }

    soft_link &operator=(soft_link &&other) noexcept {
        if (this == &other)
            return *this;
        base_data::operator =(std::move(other));
        path_ptr = std::move(other.path_ptr);
        return *this;
    }

    static void set_soft_link_path(soft_link *link, const path &path);
};


#endif //SOFT_LINK_H
