#ifndef FS_NODE_OPS_H
#define FS_NODE_OPS_H

#include <memory>

#include "loaded_node.h"
#include "src/hierarchy/base/node/node.h"
#include "src/hierarchy/data/node_data.h"

struct fs_node_ops {

    virtual ~fs_node_ops() {
    }

    /** regarding nodes */
    // allcoate a new node in the filesystem under a unique id.
    // return its id
    virtual node_id allocte_node() = 0;

    // free a node with the specified id from the filesystem
    virtual void free_node(node_id id) = 0;

    // returns true if the specified node exists, false otherwise
    virtual bool has_node(node_id id) = 0;

    // read from the filesystem the current data of the node with the specific id.
    // return its data (allocated locally), or nullptr if node doesn't exist
    virtual loaded_node read_node(node_id id, bool node_name, bool node_data) = 0;

    // update in the filesystem the corresponding data of the node with the specified id (if id exists)
    virtual void update_node(const node *node) = 0;

    /** regarding node data */
    virtual node_data_id allocate_node_data() = 0;

    virtual void free_node_data(node_data_id id) = 0;

    virtual bool has_node_data(node_data_id id) = 0;

    virtual std::unique_ptr<node_data> read_node_data(node_data_id id) = 0;

    virtual void update_node_data(const node_data *node) = 0;

};

#endif //FS_NODE_OPS_H
