#ifndef FS_H
#define FS_H

#include <memory>

#include "fs_id.h"

#include "loaded_node.h"
#include "src/hierarchy/refs/node_refs.h"
#include "src/hierarchy/node/node.h"
#include "src/hierarchy/node/node_id.h"
#include "src/hierarchy/path/path.h"
#include "src/hierarchy/data/node_data.h"

struct fs {
    friend struct dir;
    friend struct soft_link;
    friend struct text_file;

    fs_id id;

    explicit fs(fs_id id)
        : id(id) {
    }

    virtual ~fs() {}

    /** private, internal filesystem functions */
private:
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
    virtual loaded_node read_node(node_id id) = 0;

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

    /** filesystem api */
private:
    // create a node in the path comp directly down from the given root node
    node_id mk_direct_node(node_id parent, const path_comp &comp, const data &val);

    // search the path starting from the given root node, create any intermediate nodes if needed
    node_id mk_path(node_id id, const path &path, const std::vector<data> &data);

    // search the path going down from the given root node, create any intermediate directory nodes if needed, and create the end node as given
    node_id mk_node(node_id id, const path &path, const data &val);

    // read the node's data
    std::unique_ptr<node_data> stat_node_data(node_data_id id);

public:
    // all filesystems should have a single root node
    virtual node_id get_root_node_id() = 0;

    /** search for a node by a path */
    // search the path component going down from the given root node
    node_id search_path_comp(node_id parent, const path_comp &comp);

    // search the path going down from the given root node
    node_id search_path(node_id parent, const path &path);

    /** test if a path exists (points to a valid node) */
    inline bool path_comp_exists(node_id parent, const path_comp &comp) {
        return search_path_comp(parent, comp) != node_id_null;
    }

    inline bool path_exists(node_id parent, const path &path) {
        return search_path(parent, path) != node_id_null;
    }

    /** retrieve info about a node and its data */
    loaded_node stat_node(node_id node);

    node_data_type stat_node_data_type(node_id id);

    /** directory specific funcs */
    // search the path going down from the given root node, create any intermediate directory nodes if needed, and create an empty dir at the end
    std::unique_ptr<dir> stat_dir(node_id id);

    node_id mk_dir(node_id parent, const path &path);

    /** textfile specific funcs */
    std::unique_ptr<text_file> open_textfile(node_id id);

    void close_textfile(text_file* file);

    node_id mk_textfile(node_id parent, const path &path, const text_file &data);

    /* char dev... */
    /* ... */

    /** softlink specific funcs */
    std::unique_ptr<soft_link> stat_softlink(node_id id);

    node_id follow_softlink(node_id link_id, int max_steps = 0);

    node_id mk_softlink(node_id root, const path &path, const soft_link &link);

    /** delete nodes */
    void rm_node(node_id id, bool recursive = false);

    /** hooks over nodes */
};


#endif //FS_H
