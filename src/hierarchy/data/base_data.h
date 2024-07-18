#ifndef BASE_DATA_H
#define BASE_DATA_H

#include "src/hierarchy/data/node_data_id.h"

struct fs;
struct base_data {
    node_data_id id;

    base_data() : id(node_data_id_null) {}

    explicit base_data(node_data_id id)
        : id(id) {
    }

    virtual ~base_data() {}

    base_data(const base_data &other)
        : id(other.id) {
    }

    base_data(base_data &&other) noexcept
        : id(other.id) {
    }

    base_data & operator=(const base_data &other) {
        if (this == &other)
            return *this;
        id = other.id;
        return *this;
    }

    base_data & operator=(base_data &&other) noexcept {
        if (this == &other)
            return *this;
        id = other.id;
        return *this;
    }
};

#endif //BASE_DATA_H
