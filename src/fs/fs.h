#ifndef FS_H
#define FS_H

#include "fs_node_ops.h"
#include "fs_ops.h"

struct fs : private fs_node_ops, public fs_ops {
private:
    node_id mknode(node_id root, const path_comp &comp, const node_data_var &data) override;

    node_id mkpath(node_id parent, const path& path, const vector<node_data_var> &data) override;

public:
    node_id search_comp(node_id root, const path_comp &comp) override;

    node_id search_path(node_id root, const path &path) override;

    node_id mk_dir(node_id root, const path &path) override;

    node_id mk_obj(node_id root, const path_comp &comp, const obj_data &data) override;

    loaded_node stat_node(node_id node) override;

};


#endif //FS_H
