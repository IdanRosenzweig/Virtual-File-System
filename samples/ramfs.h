#ifndef RAMFS_H
#define RAMFS_H

#include <map>
#include "lib/id_manager.h"

#include "src/fs/fs.h"
#include "src/hierarchy/dir/dir.h"

class ramfs : public fs  {
private:
    id_manager<node_id> id_gen;

    // allocate locally
    node_id root_id;

    // the fs implemenntation for allocating and freeing nodes would just be allocating nodes locally on the ram and searching them using search tree
    std::map<node_id, node> nodes;

public:
    ramfs() : id_gen(node_id_null + 1) {
        // no need to point to some device or hardware, this fs will just allocate locally on the ram
        // initialize the filesystem with a root node of type directory (common...)
        root_id = allocte_node();
        nodes[root_id].data = dir((fs*) this, root_id);
    }

    ~ramfs() {
        // for (const auto& iter : nodes) {
            // const node& node = iter.second;

            // void* data_ptr = node.data;
            // if (data_ptr != nullptr) {
            //     switch (node.type) {
            //         case DIR: {
            //             delete reinterpret_cast<directory*>(data_ptr);
            //             break;
            //         }
            //     }
            // }
        // }
    }

    node_id allocte_node() override;

    void free_node(node_id id) override;

    bool has_node(node_id id) override;

    std::unique_ptr<node> read_node(node_id id) override;

    void update_node(const node *node) override;

    node_id get_root_node_id() override;
};


#endif //RAMFS_H
