#ifndef RAMFS_H
#define RAMFS_H

#include <map>
#include "lib/id_manager.h"

#include "src/fs/fs.h"
#include "src/hierarchy/data/dir/dir.h"

class ramfs : public fs {
private:
    id_manager<node_id> nodes_id_gen;
    id_manager<node_data_id> data_id_gen;
    id_manager<node_data_id> refs_id_gen;

    // allocate locally
    node_id root_id;

    // the fs implemenntation for allocating and freeing nodes would just be allocating nodes locally on the ram and searching them using search tree
    std::map<node_id, node> nodes_pool;
    std::map<node_data_id, node_data> data_pool;
    std::map<node_refs_id, node_refs> refs_pool;

public:
    explicit ramfs(fs_id id)
        : fs(id)
          , nodes_id_gen(node_id_null + 1), data_id_gen(node_data_id_null + 1), refs_id_gen(node_refs_id_null + 1) {
        // no need to provide some device or hardware in the constructor, this fs will just allocate locally on the ram

        // initialize the filesystem with a root node of type directory

        // allocte a root node
        root_id = allocte_node();
        node root_node(root_id);

        // allocte a directory as the root node
        node_data_id root_dir_id = allocate_node_data();
        node_data root_dir{dir(root_dir_id)};

        // allocte empty refs
        node_refs_id refs_id = allocte_node_refs();
        node_refs refs(refs_id);

        // write node
        root_node.data_id = root_dir_id;
        root_node.refs_id = refs_id;
        write_node(&root_node);

        // write data
        write_node_data(&root_dir);

        // write refs
        write_node_refs(&refs);
    }

    ~ramfs() {
    }

private:
    node_id allocte_node() override;

    void free_node(node_id id) override;

    bool has_node(node_id id) override;

    loaded_node read_node(node_id id) override;

    void write_node(const node *node) override;

    node_data_id allocate_node_data() override;

    void free_node_data(node_data_id id) override;

    bool has_node_data(node_data_id id) override;

    node_data_type read_node_data_type(node_data_id id) override;

    std::unique_ptr<node_data> read_node_data(node_data_id id) override;

    void write_node_data(const node_data *data) override;

    node_refs_id allocte_node_refs() override;

    void free_node_refs(node_refs_id id) override;

    bool has_node_refs(node_refs_id id) override;

    std::unique_ptr<node_refs> read_node_refs(node_refs_id id) override;

    void write_node_refs(const node_refs *refs) override;

public:
    node_id get_root_node_id() override;
};


#endif //RAMFS_H
