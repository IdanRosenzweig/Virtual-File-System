#ifndef RAM_DRIVER_H
#define RAM_DRIVER_H

#include <map>
#include "lib/id_manager.h"

#include "src/hierarchy/basic_driver.h"

class ram_driver : public basic_driver {
private:
    id_manager<node_id_t> nodes_id_gen;
    id_manager<comp_id_t> comps_id_gen;
    id_manager<content_id_t> refs_id_gen;
    id_manager<content_id_t> content_id_gen;

    // the fs implemenntation for allocating and freeing nodes would just be allocating nodes locally on the ram and searching them using search tree
    std::map<node_id_t, node> nodes_pool;
    std::map<comp_id_t, comp> comps_pool;
    std::map<refs_id_t, refs> refs_pool;
    std::map<content_id_t, content> content_pool;

public:
    explicit ram_driver()
        : nodes_id_gen(node_id_null + 1), comps_id_gen(comp_id_null + 1), refs_id_gen(refs_id_null + 1),
          content_id_gen(content_id_null + 1) {
        // no need to provide some device or hardware in the constructor, this fs will just allocate locally on the ram
    }


    node_id_t allocte_node() override;

    void deallocate_node(node_id_t id) override;

    bool has_node(node_id_t id) override;

    std::unique_ptr<node> read_node(node_id_t id) override;

    void write_node(const node *node) override;


    comp_id_t allocate_comp() override;

    void deallocate_comp(comp_id_t id) override;

    bool has_comp(comp_id_t id) override;

    comp_type read_comp_type(comp_id_t id) override;

    std::unique_ptr<comp> read_comp(comp_id_t id) override;

    void write_comp(const comp *node) override;


    refs_id_t allocte_refs() override;

    void deallocate_refs(refs_id_t id) override;

    bool has_refs(refs_id_t id) override;

    std::unique_ptr<refs> read_refs(refs_id_t id) override;

    void write_refs(const refs *refs) override;


    content_id_t allocate_content() override;

    void deallocate_content(content_id_t id) override;

    bool has_content(content_id_t id) override;

    content_type read_content_type(content_id_t id) override;

    std::unique_ptr<content> read_content(content_id_t id) override;

    void write_content(const content *data) override;
};


#endif //RAM_DRIVER_H
