#ifndef NODE_H
#define NODE_H

#include <cstdint>
#include <string>
#include <variant>

#include "node_id.h"
#include "node_name.h"
#include "node_data.h"
#include "lib/lib_common.h"

struct node_fs_related {
    fs *owner_fs; // pointer to the owning filesytem
    node_id id; // unique id given for each node. this value doesn't change
};

struct node {
    node_fs_related fs_related;

    node_name name = node_name_null; // non unique name for the node. currently this value doesn't change
    node_data data = null_node(); // polymorphic data
    static constexpr inline node_data_type data_type(const node &node) {
        switch (node.data.index()) {
            default:
            case variant_index<null_node, node_data>: return node_data_type::NULL_NODE;
            case variant_index<dir, node_data>: return node_data_type::DIR;
            case variant_index<object, node_data>: return node_data_type::OBJ;
        }
    }

    node();

    explicit node(const node_fs_related &fs_related);

    node(const node &other);

    node(node &&other) noexcept;

    node &operator=(const node &other);

    node &operator=(node &&other) noexcept;

    // equality and relational operators to compare nodes
    friend bool operator==(const node &lhs, const node &rhs) {
        return lhs.fs_related.id == rhs.fs_related.id;
    }

    friend bool operator!=(const node &lhs, const node &rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const node &lhs, const node &rhs) {
        return lhs.fs_related.id < rhs.fs_related.id;
    }

    friend bool operator<=(const node &lhs, const node &rhs) {
        return !(rhs < lhs);
    }

    friend bool operator>(const node &lhs, const node &rhs) {
        return rhs < lhs;
    }

    friend bool operator>=(const node &lhs, const node &rhs) {
        return !(lhs < rhs);
    }
};

#endif //NODE_H
