#ifndef NODE_DATA_H
#define NODE_DATA_H

#include <variant>
#include "lib/lib_common.h"

#include "node_data_id.h"

#include "null_data/null_data.h"
#include "dir/dir.h"
#include "soft_link/soft_link.h"
#include "text_file/text_file.h"
#include "mount/mount.h"

enum class node_data_type {
    NULL_DATA, DIR, SOFT_LINK, TEXTFILE, MOUNT
};

// all possible types of node data
using data = std::variant<null_data, dir, soft_link, text_file, mount>; // polymorphic data

struct node_data : public data {
    static constexpr inline node_data_type data_type(const node_data &val) {
        switch (val.index()) {
            default:
            case variant_index<null_data, data>: return node_data_type::NULL_DATA;
            case variant_index<dir, data>: return node_data_type::DIR;
            case variant_index<soft_link, data>: return node_data_type::SOFT_LINK;
            case variant_index<text_file, data>: return node_data_type::TEXTFILE;
            case variant_index<mount, data>: return node_data_type::MOUNT;
        }
    }

    static constexpr inline node_data_id data_id(const node_data &val) {
        return std::visit([&](auto &obj) { return obj.id; }, val);
    }

    template<typename T>
    static constexpr inline T* get_ptr(const node_data &val) {
        return (T*) std::get_if<T>(&val);
    }

    node_data() : data(null_data()) {
    }

    explicit node_data(const data &val)
        : data(val) {
    }

    node_data(const node_data &other)
        : data(other) {
    }

    node_data(node_data &&other) noexcept
        : data(std::move(other)) {
    }

    node_data &operator=(const node_data &other) {
        if (this == &other)
            return *this;
        data::operator =(other);
        return *this;
    }

    node_data &operator=(node_data &&other) noexcept {
        if (this == &other)
            return *this;
        data::operator =(std::move(other));
        return *this;
    }
};


#endif //NODE_DATA_H
