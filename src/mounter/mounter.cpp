#include "mounter.h"

node_id_t mounter::get_root_node_id() {
    return root_id;
}

std::unique_ptr<mount> mounter::stat_mount(node_id_t id) {
    auto content = stat_content(stat_node(id)->content_id);
    return std::make_unique<mount>(*content::get_ptr<mount>(content.get()));
}

node_id_t mounter::mk_mount(node_id_t parent, const path &p, const mount &mnt) {
    if (!p.empty()) {
        for (int i = 0; i < p.size() - 1 && parent != node_id_null; i++) {
            if (!path_comp_exists(parent, p[i])) parent = mk_direct_node(parent, p[i], comp(dir()), content(null_content())); // make empty directories
            else {
                if (stat_content_type(parent) == content_type::MOUNT) {
                    auto par_mnt_ptr = stat_mount(parent);
                    par_mnt_ptr->no_sub_mount++;

                    content par_mnt(std::move(*par_mnt_ptr));
                    par_mnt_ptr.reset();

                    driver->write_content(&par_mnt);
                }
            }
        }

        // if (parent != node_id_null) {
            if (!path_comp_exists(parent, p.back())) parent = mk_direct_node(parent, p.back(), comp(dir()), content(mnt));
            else parent = search_path_comp(parent, p.back());
        // }
    }
    if (parent == node_id_null) return node_id_null;

    auto node = driver->read_node(parent);
    if (driver->read_content_type(node->content_id) != content_type::NULL_CONTENT) return node_id_null; // already mounted

    // allocate mount
    mount mounted(mnt);
    content_id_t id = driver->allocate_content();
    mounted.id = id;
    content mnt_content(std::move(mounted));
    // write mount
    driver->write_content(&mnt_content);

    // update the node
    node->content_id = id;
    // and write it as well
    driver->write_node(node.get());

    return parent;
}

node_id_t mounter::mv_mount(node_id_t parent, const path &src, const path &dest) {
    node_id_t src_id = search_path(parent, src);
    if (src_id == node_id_null) return node_id_null;
    auto src_node = driver->read_node(src_id);
    if (driver->read_content_type(src_node->content_id) != content_type::MOUNT) return node_id_null; // source is not mounted

    node_id_t dest_id = mk_node(parent, dest, comp(dir()), content(null_content())); // make in case not exist
    if (dest_id == node_id_null) return node_id_null;
    auto dest_node = driver->read_node(dest_id);
    if (driver->read_content_type(dest_node->content_id) != content_type::NULL_CONTENT) return node_id_null; // dest is already mounted

    // move the content id
    dest_node->content_id = src_node->content_id;
    src_node->content_id = content_id_null;

    // write the nodes
    driver->write_node(src_node.get());
    driver->write_node(dest_node.get());

    return dest_id;
}

node_id_t mounter::cp_mount(node_id_t parent, const path &src, const path &dest) {
    node_id_t src_id = search_path(parent, src);
    if (src_id == node_id_null) return node_id_null;
    auto src_node = driver->read_node(src_id);
    if (driver->read_content_type(src_node->content_id) != content_type::MOUNT) return node_id_null; // source is not mounted

    node_id_t dest_id = mk_node(parent, dest, comp(dir()), content(null_content())); // make in case not exist
    if (dest_id == node_id_null) return node_id_null;
    auto dest_node = driver->read_node(dest_id);
    if (driver->read_content_type(dest_node->content_id) != content_type::NULL_CONTENT) return node_id_null; // dest is already mounted

    // make copy of the mount content
    content_id_t content_copy_id = driver->allocate_content();
    auto mnt = stat_content(src_node->content_id);
    mount mnt_copy(*content::get_ptr<mount>(mnt.get()));
    mnt_copy.id = content_copy_id;

    // write the content
    content content_copy(mnt_copy);
    driver->write_content(&content_copy);

    // assign the content to the dest node
    dest_node->content_id = content_copy_id;

    // write the dest node
    driver->write_node(dest_node.get());

    return dest_id;
}

void mounter::rm_mount(node_id_t parent, const path &p) {
    node_id_t id = search_path(parent, p);
    if (id == node_id_null) return;

    auto node = driver->read_node(parent);
    if (driver->read_content_type(node->content_id) != content_type::MOUNT) return; // not mouned

    // deallocate mount content
    driver->deallocate_content(node->content_id);

    // update the node
    node->content_id = content_id_null;
    // and write it as well
    driver->write_node(node.get());
}

std::pair<content_fs*, path> mounter::search_underlying_mount(node_id_t root, const path &p) {
    std::pair<content_fs*, path> res;

    auto node = driver->read_node(root);
    if (driver->read_content_type(node->content_id) == content_type::MOUNT) {
        auto content = driver->read_content(node->content_id);
        res = std::make_pair(content::get_ptr<mount>(content.get())->mnt_fs, p);
        if (content::get_ptr<mount>(content.get())->no_sub_mount == 0) return res;
    } else
        res = std::make_pair(nullptr, path{});

    for (int i = 0; i < p.size() && root != node_id_null; i++) {
        root = search_path_comp(root, p[i]);
        auto node = driver->read_node(root);
        if (root != node_id_null && driver->read_content_type(node->content_id) == content_type::MOUNT) {
            auto content = driver->read_content(node->content_id);
            res = std::make_pair(content::get_ptr<mount>(content.get())->mnt_fs, path(p.begin() + i, p.end()));
            if (content::get_ptr<mount>(content.get())->no_sub_mount == 0) return res;
        }
    }

    return res;
}
