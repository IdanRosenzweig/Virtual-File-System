#ifndef NULL_COMP_H
#define NULL_COMP_H

#include "../base_comp.h"

struct null_comp : public base_comp {
    null_comp() : base_comp() {
    }

    explicit null_comp(comp_id_t id)
        : base_comp(id) {
    }

    friend bool operator==(const null_comp &lhs, const null_comp &rhs) {
        return true;
    }

    friend bool operator!=(const null_comp &lhs, const null_comp &rhs) {
        return false;
    }

    friend bool operator<(const null_comp &lhs, const null_comp &rhs) {
        return false;
    }

    friend bool operator<=(const null_comp &lhs, const null_comp &rhs) {
        return true;
    }

    friend bool operator>(const null_comp &lhs, const null_comp &rhs) {
        return false;
    }

    friend bool operator>=(const null_comp &lhs, const null_comp &rhs) {
        return true;
    }

    friend size_t hash_value(const null_comp &obj) {
        return 6969;
    }
};

#endif //NULL_COMP_H
