#ifndef RAM_DRIVER_H
#define RAM_DRIVER_H

#include <map>
#include "lib/id_manager.h"

#include "src/hierarchy/basic_driver.h"

class ram_driver : public basic_driver {
private:
    id_manager<node_id_t> nodes_id_gen;
    id_manager<comp_id_t> comps_id_gen;
    id_manager<refs_id_t> refs_id_gen;
    id_manager<content_id_t> content_id_gen;

    // the implemenntation for allocating and freeing nodes would just be allocating nodes locally on the ram and searching them using search tree
    std::map<node_id_t, node_t> nodes_pool;
    std::map<comp_id_t, comp_t> comps_pool;
    std::map<refs_id_t, refs_t> refs_pool;
    std::map<content_id_t, content_t> content_pool;

public:
    explicit ram_driver()
        : nodes_id_gen(min_accessable_node_id), comps_id_gen(min_accessable_comp_id),
          refs_id_gen(min_accessable_refs_id),
          content_id_gen(min_accessable_content_id) {
        // no need to provide some device or hardware in the constructor, will just allocate locally on the ram
    }


    node_id_t allocte_node() noexcept override;

    void deallocate_node(node_id_t id) noexcept override;

    bool has_node(node_id_t id) noexcept override;

    void read_node(node_id_t id, node_t *dest) noexcept override;

    void write_node(const node_t *node) noexcept override;


    comp_id_t allocate_comp() noexcept override;

    void deallocate_comp(comp_id_t id) noexcept override;

    bool has_comp(comp_id_t id) noexcept override;

    comp_type read_comp_type(comp_id_t id) noexcept override;

    void read_comp(comp_id_t id, comp_t *dest) noexcept override;

    void write_comp(const comp_t *node) noexcept override;


    refs_id_t allocte_refs() noexcept override;

    void deallocate_refs(refs_id_t id) noexcept override;

    bool has_refs(refs_id_t id) noexcept override;

    void read_refs(refs_id_t id, refs_t *dest) noexcept override;

    void write_refs(const refs_t *refs) noexcept override;


    content_id_t allocate_content() noexcept override;

    void deallocate_content(content_id_t id) noexcept override;

    bool has_content(content_id_t id) noexcept override;

    content_type read_content_type(content_id_t id) noexcept override;

    void read_content(content_id_t id, content_t *dest) noexcept override;

    void write_content(const content_t *data) noexcept override;
};


#endif //RAM_DRIVER_H
