#ifndef LOADED_NODE_H
#define LOADED_NODE_H

#include <iostream>
#include <memory>
#include "src/hierarchy/base/node/node.h"

struct fs;

struct loaded_node {
    fs* owner_fs;
    std::unique_ptr<node> ptr;

    loaded_node(fs *owner_fs, std::unique_ptr<::node> &&ptr)
        : owner_fs(owner_fs),
          ptr(std::move(ptr)) {
    }

    // loaded_node(const loaded_node &other)
        // : owner_fs(other.owner_fs),
          // ptr(other.ptr) {
    // }

    loaded_node(loaded_node &&other) noexcept
        : owner_fs(other.owner_fs),
          ptr(std::move(other.ptr)) {
    }

    // loaded_node & operator=(const loaded_node &other) {
    //     if (this == &other)
    //         return *this;
    //     owner_fs = other.owner_fs;
    //     ptr = other.ptr;
    //     return *this;
    // }

    loaded_node & operator=(loaded_node &&other) noexcept {
        if (this == &other)
            return *this;
        owner_fs = other.owner_fs;
        ptr = std::move(other.ptr);
        return *this;
    }
};
#endif //LOADED_NODE_H
