#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

#include "lib/lib_common.h"

#include "node_id.h"
#include "node_name.h"
#include "src/hierarchy/comp/comp_id.h"
#include "src/hierarchy/content/content_id.h"
#include "src/hierarchy/refs/refs_id.h"

struct node {
    node_id_t id = node_id_null; // constant, unique id given for each node
    node_name name = node_name_null; // non unique name for the node

    comp_id_t comp_id = comp_id_null; // pointer to a unique component struct representing this node

    refs_id_t refs_id = refs_id_null; // pointer to unique references struct
    content_id_t content_id = content_id_null; // pointer to non unique content related to this node

    node() = default;

    explicit node(node_id_t id) : id(id) {
    }

    node(const node &other)
        : id(other.id),
          name(other.name),
          comp_id(other.comp_id),
          refs_id(other.refs_id),
          content_id(other.content_id) {
    }

    node(node &&other) noexcept
        : id(other.id),
          name(std::move(other.name)),
          comp_id(other.comp_id),
          refs_id(other.refs_id),
          content_id(other.content_id) {
        other.id = node_id_null;
        other.name = node_name_null;
        other.comp_id = comp_id_null;
        other.refs_id = refs_id_null;
        other.content_id = content_id_null;
    }

    node & operator=(const node &other) {
        if (this == &other)
            return *this;
        id = other.id;
        name = other.name;
        comp_id = other.comp_id;
        refs_id = other.refs_id;
        content_id = other.content_id;
        return *this;
    }

    node & operator=(node &&other) noexcept {
        if (this == &other)
            return *this;
        id = other.id;
        name = std::move(other.name);
        comp_id = other.comp_id;
        refs_id = other.refs_id;
        content_id = other.content_id;
        other.id = node_id_null;
        other.name = node_name_null;
        other.comp_id = comp_id_null;
        other.refs_id = refs_id_null;
        other.content_id = content_id_null;
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
