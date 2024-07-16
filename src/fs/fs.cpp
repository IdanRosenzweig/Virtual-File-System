#include "fs.h"

#include <iostream>
#include <src/hierarchy/dir/dir.h>
#include "lib/lib_common.h"


node_id fs::get_root_node_id() {
}

node_id fs::mknode(node_id root, const path_comp &comp, const node_data &data) {
    if (!has_node(root)) return node_id_null;

    std::unique_ptr<node> parent_node(read_node(root));

    switch (node::data_type(*parent_node)) {
        case node_data_type::DIR: {
            dir *dir_ptr = std::get_if<dir>(&parent_node->data);

            node_id child_id = dir::search_path_comp(dir_ptr, comp);
            if (child_id == node_id_null) {
                // need to allocte the node and attach it to the directory
                // allocate a new node
                child_id = allocte_node();
                // set its data and name
                std::unique_ptr<node> child = read_node(child_id);
                child->name = comp;
                child->data = data;
                // update it on the fs
                update_node(child.get());

                // add the new node under the current directory
                dir::add_child_to_dir(dir_ptr, child_id);
                // and update the current directory
                update_node(parent_node.get());
            }

            return child_id;
        }
        default: return node_id_null;
    }
}

node_id fs::mkpath(node_id parent, const path &path, const vector<node_data> &data) {
    if (!has_node(parent)) return node_id_null;

    node_id curr_id = parent;
    for (int i = 0; i < path.size() && curr_id != node_id_null; i++)
        curr_id = mknode(curr_id, path[i], data[i]);

    return curr_id;
}

node_id fs::search_comp(node_id root, const path_comp &comp) {
    if (!has_node(root)) return node_id_null;

    std::unique_ptr<node> parent_node(read_node(root));
    switch (node::data_type(*parent_node)) {
        case node_data_type::DIR: {
            dir *dir_ptr = std::get_if<dir>(&parent_node->data);
            return dir::search_path_comp(dir_ptr, comp);
        }
        default: return node_id_null;
    }
}

node_id fs::search_path(node_id root, const path &path) {
    if (!has_node(root)) return node_id_null;

    node_id curr_id = root;
    for (int i = 0; i < path.size() && curr_id != node_id_null; i++)
        curr_id = search_comp(curr_id, path[i]);

    return curr_id;
}

node_id fs::mk_dir(node_id root, const path &path) {
    if (!has_node(root)) return node_id_null;

    node_id curr_id = root;
    for (int i = 0; i < path.size() && curr_id != node_id_null; i++) {
        curr_id = mknode(curr_id, path[i], node_data(dir{this, curr_id}));
    }

    return curr_id;
}

node_id fs::mk_obj(node_id root, const path_comp &comp, const obj_data &data) {
    object obj{this, root};
    obj.data = data;
    return mknode(root, comp, node_data{std::move(obj)});
}

std::unique_ptr<node> fs::stat_node(node_id node) {
    return std::move(this->read_node(node));
}
