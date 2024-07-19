#ifndef REFS_H
#define REFS_H

#include <set>

#include "refs_id.h"
#include "src/hierarchy/node/node_id.h"

struct refs {
    refs_id_t id = refs_id_null;

    std::set<node_id_t> dirs;
    // hard links? ...

    refs() = default;

    explicit refs(refs_id_t id): id(id) {
    }
};

#endif //REFS_H
