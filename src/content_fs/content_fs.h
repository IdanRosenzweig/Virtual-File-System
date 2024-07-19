#ifndef CONTENT_FS_H
#define CONTENT_FS_H

#include "../content_fs/fs_id.h"
#include "../hierarchy/basic_hierarchy.h"

struct content_fs : public basic_hierarchy {
private:
    node_id_t root_id;

public:
    fs_id id;

    content_fs(fs_id id, basic_driver *driver)
        : id(id), basic_hierarchy(driver) {
        // initialize the filesystem with a root node of type directory

        // allocte a root node
        root_id = driver->allocte_node();
        node root_node(root_id);

        // allocte a directory as the root comp
        comp_id_t comp_id = driver->allocate_comp();
        struct dir dir{};
        dir.id = comp_id;
        comp root_dir{dir};

        // allocte empty refs
        refs_id_t refs_id = driver->allocte_refs();
        refs refs(refs_id);

        // write the new node and all its data
        root_node.comp_id = comp_id;
        root_node.refs_id = refs_id;
        root_node.content_id = content_id_null;
        driver->write_node(&root_node);
        driver->write_comp(&root_dir);
        driver->write_refs(&refs);
    }

    node_id_t get_root_node_id() override;

    /** directory specific funcs */
    std::unique_ptr<dir> stat_dir(node_id_t id);

    // search the path going down from the given root node, create any intermediate directory nodes if needed, and create an empty dir at the end
    // (this function is just a wrapper around mk_node)
    node_id_t mk_dir(node_id_t parent, const path &path);

    /** textfile specific funcs */
    // search the path going down from the given root node, create any intermediate directory nodes if needed, and create an empty textfile at the end
    // (this function is just a wrapper around mk_node)
    node_id_t mk_textfile(node_id_t parent, const path &path);

    std::unique_ptr<textfile> open_textfile(node_id_t id);

    void close_textfile(textfile *file);

    /** softlink specific funcs */
    std::unique_ptr<softlink> stat_softlink(node_id_t id);

    node_id_t follow_softlink(node_id_t link_id, int max_steps = 0);

    node_id_t mk_softlink(node_id_t root, const path &path, const softlink &link);

};



#endif //CONTENT_FS_H
