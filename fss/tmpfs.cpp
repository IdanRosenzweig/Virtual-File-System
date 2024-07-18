#include "tmpfs.h"

tmpfs::tmpfs(fs_id id, fs_driver_ops *driver): fs(id, driver) {
    // initialize the filesystem with a root node of type directory

    // allocte a root node
    root_id = driver->allocte_node();
    node root_node(root_id);

    // allocte a directory as the root node
    node_data_id root_dir_id = driver->allocate_node_data();
    struct dir dir{};
    dir.id = root_dir_id;
    node_data root_dir{dir};

    // allocte empty refs
    node_refs_id refs_id = driver->allocte_node_refs();
    node_refs refs(refs_id);

    // write node
    root_node.data_id = root_dir_id;
    root_node.refs_id = refs_id;
    driver->write_node(&root_node);

    // write data
    driver->write_node_data(&root_dir);

    // write refs
    driver->write_node_refs(&refs);
}

node_id tmpfs::get_root_node_id() {
    return root_id;
}
