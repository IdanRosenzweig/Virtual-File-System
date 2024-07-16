#ifndef OBJECT_H
#define OBJECT_H

// #include "../node/node_id.h"
#include "obj_data.h"
#include "lib/lib_common.h"

#include "text_file/text_file.h"

// struct fs;

struct object {
private:
    // node_id base_node;

public:
    obj_data data;
    static constexpr inline obj_data_type data_type(const object &obj) {
        switch (obj.data.index()) {
            default:
            case variant_index<text_file, obj_data>: return obj_data_type::TEXT_FILE;
        }
    }

    // object(node_id base_node)
        // : base_node(base_node) {
    // }
    explicit object(const obj_data &data)
        : data(data) {
    }
};

#endif //OBJECT_H
