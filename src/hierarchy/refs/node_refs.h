#ifndef NODE_REFS_H
#define NODE_REFS_H

#include <set>

#include "node_refs_id.h"
#include "src/hierarchy/node/node_id.h"

struct node_refs {
    node_refs_id id;

    std::set<node_id> dirs;
    // hard links? ...

    node_refs() : id(node_refs_id_null), dirs() {

    }

    explicit node_refs(node_refs_id id)
        : id(id), dirs() {
    }
};

#endif //NODE_REFS_H
