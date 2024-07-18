#ifndef NULL_DATA_H
#define NULL_DATA_H

#include <cstddef>

#include "../base_data.h"

struct null_data : public base_data {

    null_data() : base_data() {}

    explicit null_data(node_data_id id)
        : base_data(id) {
    }

    friend bool operator==(const null_data &lhs, const null_data &rhs) {
        return true;
    }

    friend bool operator!=(const null_data &lhs, const null_data &rhs) {
        return false;
    }

    friend bool operator<(const null_data &lhs, const null_data &rhs) {
        return false;
    }

    friend bool operator<=(const null_data &lhs, const null_data &rhs) {
        return true;
    }

    friend bool operator>(const null_data &lhs, const null_data &rhs) {
        return false;
    }

    friend bool operator>=(const null_data &lhs, const null_data &rhs) {
        return true;
    }

    friend std::size_t hash_value(const null_data &obj) {
        return 6969;
    }
};

#endif //NULL_DATA_H
