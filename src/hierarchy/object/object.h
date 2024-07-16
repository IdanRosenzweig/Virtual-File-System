#ifndef OBJECT_H
#define OBJECT_H

#include "../node/node_id.h"
#include "obj_data.h"
#include "lib/lib_common.h"

struct fs;

struct object {
private:
    fs *owner_fs; // pointer to the owning filesytem
    node_id base_node;

public:
    obj_data data;
    static constexpr inline obj_data_type data_type(const object &obj) {
        switch (obj.data.index()) {
            default:
            case variant_index<text_file, obj_data>: return obj_data_type::TEXT_FILE;
        }
    }

    object(fs *owner_fs, node_id base_node)
        : owner_fs(owner_fs),
          base_node(base_node) {
    }
};

#endif //OBJECT_H
