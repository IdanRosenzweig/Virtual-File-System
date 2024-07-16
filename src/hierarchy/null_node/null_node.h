#ifndef NULL_NODE_H
#define NULL_NODE_H

#include <cstddef>

struct null_node {

    friend bool operator==(const null_node &lhs, const null_node &rhs) {
        return true;
    }

    friend bool operator!=(const null_node &lhs, const null_node &rhs) {
        return false;
    }

    friend bool operator<(const null_node &lhs, const null_node &rhs) {
        return false;
    }

    friend bool operator<=(const null_node &lhs, const null_node &rhs) {
        return true;
    }

    friend bool operator>(const null_node &lhs, const null_node &rhs) {
        return false;
    }

    friend bool operator>=(const null_node &lhs, const null_node &rhs) {
        return true;
    }

    friend std::size_t hash_value(const null_node &obj) {
        return 6969;
    }
};

#endif //NULL_NODE_H
