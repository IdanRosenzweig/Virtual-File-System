#ifndef FS_NODE_OPS_H
#define FS_NODE_OPS_H

#include <memory>

#include "src/hierarchy/node/node.h"

struct fs_node_ops {
    // allcoate a new node in the filesystem under a unique id.
    // return its id
    virtual node_id allocte_node() = 0;

    // free a node with the specified id from the filesystem
    virtual void free_node(node_id id) = 0;

    // returns true if the specified node exists, false otherwise
    virtual bool has_node(node_id id) = 0;

    // read from the filesystem the current data of the node with the specific id.
    // return its data (allocated locally), or nullptr if node doesn't exist
    virtual std::unique_ptr<node> read_node(node_id id) = 0;

    // update in the filesystem the corresponding data of the node with the specified id (if id exists)
    virtual void update_node(const node *node) = 0;

    virtual ~fs_node_ops() {
    }
};

#endif //FS_NODE_OPS_H
