#include "basic_hierarchy.h"

node_id_t basic_hierarchy::mk_direct_node(node_id_t parent, const path_comp &p, const comp_data &comp_val,
                                          const content_data &content_val) {
    if (!driver->has_node(parent)) return node_id_null;

    parent = follow_softlink(parent);

    auto node_comp = driver->read_comp(driver->read_node(parent)->id);
    switch (comp::get_type(*node_comp)) {
        case comp_type::DIR: {
            dir *dir_ptr = comp::get_ptr<dir>(node_comp.get());

            node_id_t child_id = dir::search_path_comp(this, dir_ptr, p);
            if (child_id == node_id_null) {
                // need to allocte the node and attach it to the directory

                // allocate a new node
                child_id = driver->allocte_node();
                node child(child_id);
                child.name = p;

                // allocte comp
                comp_id_t comp_id = driver->allocate_comp();
                comp _comp(comp_val);
                std::visit([&](auto &obj) { obj.id = comp_id; }, _comp);

                // allocate its content
                content_id_t content_id = driver->allocate_content();
                content _content(content_val);
                std::visit([&](auto &obj) { obj.id = content_id; }, _content);

                // allocte empty refs
                refs_id_t refs_id = driver->allocte_refs();
                refs refs(refs_id);

                // write the new node and all its data
                child.comp_id = comp_id;
                child.refs_id = refs_id;
                child.content_id = content_id;
                driver->write_node(&child);
                driver->write_comp(&_comp);
                driver->write_refs(&refs);
                driver->write_content(&_content);

                // add the new node under the current directory
                dir::add_child_to_dir(this, dir_ptr, child_id, true);

                // write the data
                driver->write_comp(node_comp.get());
            }

            return child_id;
        }
        default: return node_id_null;
    }
}

node_id_t basic_hierarchy::mk_path(node_id_t parent, const path &p, const std::vector<comp_data> &comp_vals,
                                   const std::vector<content_data> &content_vals) {
    if (p.empty()) return node_id_null;

    for (int i = 0; i < p.size() && parent != node_id_null; i++)
        parent = mk_direct_node(parent, p[i], comp_vals[i], content_vals[i]);

    return parent;
}

node_id_t basic_hierarchy::mk_node(node_id_t parent, const path &p, const comp_data &comp_val,
                                   const content_data &content_val) {
    if (p.empty()) return node_id_null;

    for (int i = 0; i < p.size() - 1 && parent != node_id_null; i++)
        parent = mk_direct_node(parent, p[i], comp(dir()), content(null_content())); // make empty directories
    if (parent != node_id_null)
        parent = mk_direct_node(parent, p.back(), comp_val, content_val);

    return parent;
}

std::unique_ptr<comp> basic_hierarchy::stat_comp(content_id_t id) {
    return std::move(driver->read_comp((id)));
}

std::unique_ptr<content> basic_hierarchy::stat_content(content_id_t id) {
    return std::move(driver->read_content((id)));
}

node_id_t basic_hierarchy::search_path_comp(node_id_t parent, const path_comp &p) {
    if (!driver->has_node(parent)) return node_id_null;

    parent = follow_softlink(parent);

    auto node_comp = driver->read_comp(driver->read_node(parent)->id);
    switch (comp::get_type(*node_comp)) {
        case comp_type::DIR: {
            dir *dir_ptr = comp::get_ptr<dir>(node_comp.get());
            return dir::search_path_comp(this, dir_ptr, p);
        }
        default: return node_id_null;
    }
}

node_id_t basic_hierarchy::search_path(node_id_t parent, const path &p) {
    for (int i = 0; i < p.size() && parent != node_id_null; i++)
        parent = search_path_comp(parent, p[i]);

    return parent;
}

std::unique_ptr<node> basic_hierarchy::stat_node(node_id_t node) {
    return std::move(driver->read_node(node));
}

comp_type basic_hierarchy::stat_comp_type(node_id_t id) {
    return driver->read_comp_type(driver->read_node(id)->content_id);
}

content_type basic_hierarchy::stat_content_type(node_id_t id) {
    return driver->read_content_type(driver->read_node(id)->content_id);
}

node_id_t basic_hierarchy::follow_softlink(node_id_t link_id, int max_steps) {
    if (!driver->has_node(link_id)) return node_id_null;

    comp_id_t comp_id = driver->read_node(link_id)->comp_id;
    if (driver->read_comp_type(comp_id) != comp_type::SOFT_LINK) return link_id;

    int steps = 0;
    while (link_id != node_id_null && (max_steps == 0 || steps++ < max_steps)) {
        comp_id = driver->read_node(link_id)->comp_id;
        if (driver->read_comp_type(comp_id) != comp_type::SOFT_LINK) return link_id;

        auto node_comp = driver->read_comp(comp_id);
        link_id = search_path(get_root_node_id(), comp::get_ptr<softlink>(node_comp.get())->path_ptr);
    }

    return link_id;
}

void basic_hierarchy::rm_node(node_id_t id, bool recursive) {
    auto node = driver->read_node(id);
    auto comp = driver->read_comp(node->content_id);
    auto refs = driver->read_refs(node->refs_id);
    switch (comp::get_type(*comp)) {
        default:
        case comp_type::NULL_COMP: return;
        case comp_type::SOFT_LINK: {
            // update references
            for (node_id_t dir_ref: refs->dirs) {
                comp_id_t comp_id = driver->read_node(dir_ref)->comp_id;
                // switch (driver->driver->read_content(_type(dir_data_id)) {
                //     case content_type::DIR: {
                auto node_comp = driver->read_comp(comp_id);
                dir::remove_child_from_dir(this, comp::get_ptr<dir>(node_comp.get()), id, false);
                // break;
                // }
                // }
            }

            driver->deallocate_content(node->content_id); // deallocate the data
            driver->deallocate_refs(node->refs_id); // deallocate the refs
            driver->deallocate_node(id); // deallocate the node
            return;
        }
        case comp_type::DIR: {
            if (!recursive) return;

            dir *dir_ptr = comp::get_ptr<dir>(comp.get());
            // recursivly remove the child nodes
            for (node_id_t child: dir::get_all_children(this, dir_ptr))
                rm_node(child, recursive);

            driver->deallocate_comp(node->comp_id); // deallocate the comp
            driver->deallocate_refs(node->refs_id); // deallocate the refs
            driver->deallocate_content(node->content_id); // deallocate the content // todo hard links?
            driver->deallocate_node(id); // deallocate the node
            return;
        }
    }
}
