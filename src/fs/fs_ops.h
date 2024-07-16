#ifndef FS_OPS_H
#define FS_OPS_H

#include "src/hierarchy/base/node/node_id.h"
#include "src/hierarchy/base/path/path.h"
#include "src/hierarchy/data/node_data.h"

struct fs_ops {
public:
    // all filesystems should have a single root node
    virtual node_id get_root_node_id() = 0;

private:
    // create a node in the comp down from the given root node
    virtual node_id mknode(node_id root, const path_comp& comp, const poly_data &data) = 0;

    // search the path starting from the parent node, create any trailing empty nodes if needed
    virtual node_id mkpath(node_id parent, const path& path, const vector<poly_data> &data) = 0;

public:
    // search the path component going down from the given root node
    virtual node_id search_comp(node_id root, const path_comp &comp) = 0;

    // search the path going down from the given root node
    virtual node_id search_path(node_id root, const path& path) = 0;

    inline bool path_comp_exists(node_id root, const path_comp& comp) {
        return search_comp(root, comp) != node_id_null;
    }

    inline bool path_exists(node_id root, const path& path) {
        return search_path(root, path) != node_id_null;
    }

    // search the path going down from the given root node, create any trailing directory nodes if needed
    virtual node_id mk_dir(node_id root, const path& path) = 0;

    virtual node_id mk_obj(node_id root, const path_comp& comp, const obj_data& data) = 0;

    virtual loaded_node stat_node(node_id node) = 0;


};
#endif //FS_OPS_H
