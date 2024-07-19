#ifndef MOUNTER_H
#define MOUNTER_H

#include "../hierarchy/basic_hierarchy.h"
#include "../content_fs/content_fs.h"

struct mounter : private basic_hierarchy {
    node_id_t root_id;

    mounter(basic_driver *driver)
        : basic_hierarchy(driver) {
        // initialize the filesystem with a root node of type directory

        // allocte a root node
        node root_node(root_id = driver->allocte_node());

        // allocte a directory as the root comp
        comp_id_t comp_id = driver->allocate_comp();
        struct dir root_dir(comp_id);
        comp comp(std::move(root_dir));

        // allocte empty refs
        refs_id_t refs_id = driver->allocte_refs();
        refs refs(refs_id);

        // write the new node and all its data
        root_node.comp_id = comp_id;
        root_node.refs_id = refs_id;
        root_node.content_id = content_id_null; // null content
        driver->write_node(&root_node);
        driver->write_comp(&comp);
        driver->write_refs(&refs);
    }

    node_id_t get_root_node_id() override;

    /** mount specific funcs */
    std::unique_ptr<mount> stat_mount(node_id_t id);

    node_id_t mk_mount(node_id_t parent, const path &p, const mount &mnt);

    node_id_t mv_mount(node_id_t parent, const path& src, const path& dest);

    node_id_t cp_mount(node_id_t parent, const path& src, const path& dest);

    void rm_mount(node_id_t parent, const path& p);

    // regarding the target node represented by searching th given path down from the root node,
    // this function returns the local node id of the most relevant mount node that ownns the target node
    std::pair<content_fs*, path> search_underlying_mount(node_id_t root, const path &path);

};


#endif //MOUNTER_H