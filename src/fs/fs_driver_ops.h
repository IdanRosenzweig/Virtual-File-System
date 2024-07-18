#ifndef FS_DRIVER_OPS_H
#define FS_DRIVER_OPS_H

#include <memory>

#include "src/hierarchy/data/node_data.h"
#include "src/hierarchy/refs/node_refs.h"
#include "src/hierarchy/node/node.h"

struct fs_driver_ops {
    friend struct dir;
    friend struct soft_link;
    friend struct text_file;

    /** internal filesystem functions */
    /* regarding nodes */

    // allcoate a new node in the filesystem under a unique id.
    // return its id
    virtual node_id allocte_node() = 0;

    // free a node with the specified id from the filesystem
    virtual void free_node(node_id id) = 0;

    // returns true if the specified node exists, false otherwise
    virtual bool has_node(node_id id) = 0;

    // read from the filesystem the current data of the node with the specific id.
    // return its data (allocated locally), or nullptr if node doesn't exist
    // todo possible to add optional parameters (bool node_name, bool node_data, bool node_refs) to indicate whether to read thos or not
    virtual std::unique_ptr<node> read_node(node_id id) = 0;

    // update in the filesystem the corresponding data of the node with the specified id (if id exists)
    virtual void write_node(const node *node) = 0;

    /* regarding node data */

    virtual node_data_id allocate_node_data() = 0;

    virtual void free_node_data(node_data_id id) = 0;

    virtual bool has_node_data(node_data_id id) = 0;

    virtual node_data_type read_node_data_type(node_data_id id) = 0;

    virtual std::unique_ptr<node_data> read_node_data(node_data_id id) = 0;

    virtual void write_node_data(const node_data *node) = 0;

    /* regarding references */
    virtual node_refs_id allocte_node_refs() = 0;

    virtual void free_node_refs(node_refs_id id) = 0;

    virtual bool has_node_refs(node_refs_id id) = 0;

    virtual std::unique_ptr<node_refs> read_node_refs(node_refs_id id) = 0;

    virtual void write_node_refs(const node_refs *refs) = 0;
};

#endif //FS_DRIVER_OPS_H
