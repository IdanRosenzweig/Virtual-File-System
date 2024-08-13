#ifndef REFS_H
#define REFS_H

#include "lib/avl_tree.h"

#include "refs_id.h"
#include "src/node/node_id.h"

struct refs_t {
    refs_id_t id = refs_id_null;

    using avl = avl_node<ctx_t<comp_id_t>, void*>;
    avl* dirs = nullptr;

    refs_t() = default;

    explicit refs_t(refs_id_t id): id(id) {
    }
};

#endif //REFS_H
