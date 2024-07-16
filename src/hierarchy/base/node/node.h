#ifndef NODE_H
#define NODE_H

#include <string>
#include <variant>

#include "node_id.h"
#include "node_name.h"
#include "src/hierarchy/data/node_data_id.h"
#include "lib/lib_common.h"

struct node {
    node_id id; // unique id given for each node. this value doesn't change
    node_name name = node_name_null; // non unique name for the node. currently this value doesn't change

    node_data_id data_id = node_data_id_null; // pointer to node data

    node(): id(node_id_null), name(node_name_null), data_id(node_data_id_null) {
    }

    explicit node(node_id id)
        : id(id) {
    }

    node(const node &other)
        : id(other.id),
          name(other.name),
          data_id(other.data_id) {
    }

    node(node &&other) noexcept
        : id(other.id),
          name(std::move(other.name)),
          data_id(other.data_id) {
    }

    node &operator=(const node &other) {
        if (this == &other)
            return *this;
        id = other.id;
        name = other.name;
        data_id = other.data_id;
        return *this;
    }

    node &operator=(node &&other) noexcept {
        if (this == &other)
            return *this;
        id = other.id;
        name = std::move(other.name);
        data_id = other.data_id;
        return *this;
    }

    // equality and relational operators to compare nodes
    friend bool operator==(const node &lhs, const node &rhs) {
        return lhs.id == rhs.id;
    }

    friend bool operator!=(const node &lhs, const node &rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const node &lhs, const node &rhs) {
        return lhs.id < rhs.id;
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
