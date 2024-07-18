#ifndef FS_RAM_DRIVER_H
#define FS_RAM_DRIVER_H

#include <map>
#include "lib/id_manager.h"

#include "src/fs/fs_driver_ops.h"
#include "src/hierarchy/data/dir/dir.h"

class fs_ram_driver : public fs_driver_ops {
private:
    id_manager<node_id> nodes_id_gen;
    id_manager<node_data_id> data_id_gen;
    id_manager<node_data_id> refs_id_gen;

    // the fs implemenntation for allocating and freeing nodes would just be allocating nodes locally on the ram and searching them using search tree
    std::map<node_id, node> nodes_pool;
    std::map<node_data_id, node_data> data_pool;
    std::map<node_refs_id, node_refs> refs_pool;

public:
    explicit fs_ram_driver()
        : nodes_id_gen(node_id_null + 1), data_id_gen(node_data_id_null + 1), refs_id_gen(node_refs_id_null + 1) {
        // no need to provide some device or hardware in the constructor, this fs will just allocate locally on the ram
    }

    ~fs_ram_driver() {
    }

private:
    node_id allocte_node() override;

    void free_node(node_id id) override;

    bool has_node(node_id id) override;

    std::unique_ptr<node> read_node(node_id id) override;

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
};


#endif //FS_RAM_DRIVER_H
