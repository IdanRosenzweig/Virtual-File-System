#ifndef NODE_DATA_H
#define NODE_DATA_H

#include <variant>

#include "node_data_id.h"

#include "null_data/null_data.h"
#include "object/object.h"
#include "dir/dir.h"

enum class node_data_type {
    NULL_DATA, DIR, OBJ
};

using poly_data = std::variant<null_data, dir, object>; // polymorphic data

struct node_data {
    node_data_id id;
    poly_data data;

    static constexpr inline node_data_type data_type(const node_data &data) {
        switch (data.data.index()) {
            default:
            case variant_index<null_data, poly_data>: return node_data_type::NULL_DATA;
            case variant_index<dir, poly_data>: return node_data_type::DIR;
            case variant_index<object, poly_data>: return node_data_type::OBJ;
        }
    }

    node_data() : id(node_data_id_null), data(null_data()) {
    }

    explicit node_data(node_data_id id)
        : id(id), data(null_data()) {
    }

    node_data(const node_data &other)
        : id(other.id),
          data(other.data) {
    }

    node_data(node_data &&other) noexcept
        : id(other.id),
          data(std::move(other.data)) {
    }

    node_data &operator=(const node_data &other) {
        if (this == &other)
            return *this;
        id = other.id;
        data = other.data;
        return *this;
    }

    node_data &operator=(node_data &&other) noexcept {
        if (this == &other)
            return *this;
        id = other.id;
        data = std::move(other.data);
        return *this;
    }
};


#endif //NODE_DATA_H
