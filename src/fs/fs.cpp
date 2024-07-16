#include "fs.h"

#include <iostream>
#include "src/hierarchy/data/dir/dir.h"
#include "lib/lib_common.h"


node_id fs::mknode(node_id root, const path_comp &comp, const node_data_var &data) {
    if (!has_node(root)) return node_id_null;

    loaded_node root_node(std::move(read_node(root, false, true)));
    auto content = read_node_data(root_node.ptr->id);
    switch (node_data::data_type(*content)) {
        case node_data_type::DIR: {
            dir *dir_ptr = std::get_if<dir>(&content->data);

            node_id child_id = dir::search_path_comp(this, dir_ptr, comp);
            if (child_id == node_id_null) { // need to allocte the node and attach it to the directory
                // allocate a new node
                child_id = allocte_node();
                node child(child_id);
                child.name = comp;

                // allocate its data
                node_data_id data_id = allocate_node_data();
                node_data _node_data(data_id);
                _node_data.data = data;

                // update the new node and its data on the fs
                child.data_id = data_id;
                update_node(&child);
                update_node_data(&_node_data);

                // add the new node under the current directory
                dir::add_child_to_dir(this, dir_ptr, child_id);
                // and update the current directory's content
                update_node_data(content.get());
            }

            return child_id;
        }
        default: return node_id_null;
    }
}

node_id fs::mkpath(node_id parent, const path &path, const vector<node_data_var> &data) {
    if (!has_node(parent)) return node_id_null;

    node_id curr_id = parent;
    for (int i = 0; i < path.size() && curr_id != node_id_null; i++)
        curr_id = mknode(curr_id, path[i], data[i]);

    return curr_id;
}

node_id fs::search_comp(node_id root, const path_comp &comp) {
    if (!has_node(root)) return node_id_null;

    loaded_node root_node(read_node(root, false, true));
    auto content = read_node_data(root_node.ptr->id);

    switch (node_data::data_type(*content)) {
        case node_data_type::DIR: {
            dir *dir_ptr = std::get_if<dir>(&content->data);
            return dir::search_path_comp(this, dir_ptr, comp);
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
        curr_id = mknode(curr_id, path[i], node_data_var(dir{}));
    }

    return curr_id;
}

node_id fs::mk_obj(node_id root, const path_comp &comp, const obj_data &data) {
    object obj(data);
    return mknode(root, comp, node_data_var{std::move(obj)});
}

loaded_node fs::stat_node(node_id node) {
    return std::move(this->read_node(node, true, true));
}
