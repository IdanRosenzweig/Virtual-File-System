#include "fs.h"

node_id fs::mk_direct_node(node_id parent, const path_comp &comp, const data &val) {
    if (!has_node(parent)) return node_id_null;

    parent = follow_softlink(parent);

    auto content = read_node_data(read_node(parent).ptr->id);
    switch (node_data::data_type(*content)) {
        case node_data_type::DIR: {
            dir *dir_ptr = node_data::get_ptr<dir>(*content);

            node_id child_id = dir::search_path_comp(this, dir_ptr, comp);
            if (child_id == node_id_null) {
                // need to allocte the node and attach it to the directory

                // allocate a new node
                child_id = allocte_node();
                node child(child_id);
                child.name = comp;

                // allocate its data
                node_data_id data_id = allocate_node_data();
                node_data _node_data(val);
                std::visit([&](auto &obj) { obj.id = data_id; }, _node_data);

                // allocte empty refs
                node_refs_id refs_id = allocte_node_refs();
                node_refs refs(refs_id);

                // write the new node, data and refs to the fs
                child.data_id = data_id;
                child.refs_id = refs_id;
                write_node(&child);
                write_node_data(&_node_data);
                write_node_refs(&refs);

                // add the new node under the current directory
                dir::add_child_to_dir(this, dir_ptr, child_id, true);

                // write the data
                write_node_data(content.get());
            }

            return child_id;
        }
        default: return node_id_null;
    }
}

node_id fs::mk_path(node_id parent, const path &path, const std::vector<data> &data) {
    if (path.empty()) return node_id_null;

    for (int i = 0; i < path.size() && parent != node_id_null; i++)
        parent = mk_direct_node(parent, path[i], data[i]);

    return parent;
}

node_id fs::mk_node(node_id parent, const path &path, const data &val) {
    if (path.empty()) return node_id_null;

    for (int i = 0; i < path.size() - 1 && parent != node_id_null; i++)
        parent = mk_direct_node(parent, path[i], node_data(dir())); // make empty directories
    if (parent != node_id_null)
        parent = mk_direct_node(parent, path.back(), val);

    return parent;
}

std::unique_ptr<node_data> fs::stat_node_data(node_data_id id) {
    return std::move(read_node_data(id));
}


node_id fs::search_path_comp(node_id parent, const path_comp &comp) {
    if (!has_node(parent)) return node_id_null;

    parent = follow_softlink(parent);

    auto content = read_node_data(read_node(parent).ptr->id);
    switch (node_data::data_type(*content)) {
        case node_data_type::DIR: {
            dir *dir_ptr = node_data::get_ptr<dir>(*content);
            return dir::search_path_comp(this, dir_ptr, comp);
        }
        default: return node_id_null;
    }
}

node_id fs::search_path(node_id parent, const path &path) {
    if (path.empty()) return node_id_null;

    for (int i = 0; i < path.size() && parent != node_id_null; i++)
        parent = search_path_comp(parent, path[i]);

    return parent;
}

loaded_node fs::stat_node(node_id node) {
    return std::move(read_node(node));
}

node_data_type fs::stat_node_data_type(node_id id) {
    return read_node_data_type(read_node(id).ptr->data_id);
}

std::unique_ptr<dir> fs::stat_dir(node_id id) {
    return std::make_unique<dir>(*node_data::get_ptr<dir>(*stat_node_data(stat_node(id).ptr->data_id).release()));
}

node_id fs::mk_dir(node_id parent, const path &path) {
    return mk_node(parent, path, dir() /* empty dir */);
}

std::unique_ptr<text_file> fs::open_textfile(node_id id) {
    return std::make_unique<text_file>(*node_data::get_ptr<text_file>(*stat_node_data(stat_node(id).ptr->data_id)));
}

void fs::close_textfile(text_file *file) {
    node_data temp{data{*file}};
    write_node_data(&temp);
}

node_id fs::mk_textfile(node_id parent, const path &path, const text_file &data) {
    return mk_node(parent, path, data);
}

std::unique_ptr<soft_link> fs::stat_softlink(node_id id) {
    return std::make_unique<soft_link>(
        *node_data::get_ptr<soft_link>(*stat_node_data(stat_node(id).ptr->data_id).release()));
}

node_id fs::follow_softlink(node_id link_id, int max_steps) {
    if (!has_node(link_id)) return node_id_null;

    node_data_id data_id = read_node(link_id).ptr->id;
    if (read_node_data_type(data_id) != node_data_type::SOFT_LINK) return link_id;

    int steps = 0;
    while (link_id != node_id_null && (max_steps == 0 || steps++ < max_steps)) {
        data_id = read_node(link_id).ptr->id;
        switch (read_node_data_type(data_id)) {
            case node_data_type::SOFT_LINK: {
                auto content = read_node_data(data_id);
                link_id = search_path(get_root_node_id(), node_data::get_ptr<soft_link>(*content)->path_ptr);
                break;
            }
            default: return link_id;
        }
    }

    return link_id;
}

node_id fs::mk_softlink(node_id root, const path &path, const soft_link &link) {
    return mk_node(root, path, link);
}

void fs::rm_node(node_id id, bool recursive) {
    auto node = read_node(id);
    auto content = read_node_data(node.ptr->data_id);
    auto refs = read_node_refs(node.ptr->refs_id);
    switch (node_data::data_type(*content)) {
        default:
        case node_data_type::NULL_DATA: return;
        case node_data_type::TEXTFILE:
        case node_data_type::SOFT_LINK: {
            // update references
            for (node_id dir_ref: refs->dirs) {
                node_data_id dir_data_id = read_node(dir_ref).ptr->data_id;
                // switch (read_node_data_type(dir_data_id)) {
                //     case node_data_type::DIR: {
                auto content = read_node_data(dir_data_id);
                dir::remove_child_from_dir(this, node_data::get_ptr<dir>(*content), id, false);
                // break;
                // }
                // }
            }

            free_node_data(node.ptr->data_id); // free the data
            free_node_refs(node.ptr->refs_id); // free the refs
            free_node(id); // free the node
            return;
        }
        case node_data_type::DIR: {
            if (!recursive) return;

            dir *dir_ptr = node_data::get_ptr<dir>(*content);
            // recursivly remove the child nodes
            for (node_id child: dir::get_all_children(this, dir_ptr))
                rm_node(child, recursive);

            free_node_data(node.ptr->data_id); // free the data
            free_node_refs(node.ptr->refs_id); // free the refs
            free_node(id); // free the node
            return;
        }
    }
}
