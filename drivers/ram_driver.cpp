#include "ram_driver.h"

node_id_t ram_driver::allocte_node() noexcept {
    node_id_t id = nodes_id_gen.generate_id();
    nodes_pool = avl_node<node_id_t, node_t>::insert(nodes_pool, id, node_t(id));
    return id;
}

void ram_driver::deallocate_node(node_id_t id) noexcept {
    auto iter = avl_node<node_id_t, node_t>::binary_search(nodes_pool, id);
    if (iter == nullptr) return; // no such node

    nodes_pool = avl_node<node_id_t, node_t>::remove(nodes_pool, iter->value);
    nodes_id_gen.free_id(id);
}

bool ram_driver::has_node(node_id_t id) noexcept {
    return avl_node<node_id_t, node_t>::count(nodes_pool, id);
}

void ram_driver::read_node(node_id_t id, node_t *dest) noexcept {
    *dest = avl_node<node_id_t, node_t>::binary_search(nodes_pool, id)->data;
}

void ram_driver::write_node(const node_t *node) noexcept {
    auto iter = avl_node<node_id_t, node_t>::binary_search(nodes_pool, node->id);
    if (iter == nullptr) return; // the node doesn't exist

    iter->data = *node;
}


comp_id_t ram_driver::allocate_comp() noexcept {
    comp_id_t id = comps_id_gen.generate_id();
    comps_pool = avl_node<comp_id_t, comp_t>::insert(comps_pool, id, comp_t(comp_data(null_comp(id))));
    return id;
}

void ram_driver::deallocate_comp(comp_id_t id) noexcept {
    auto iter = avl_node<comp_id_t, comp_t>::binary_search(comps_pool, id);
    if (iter == nullptr) return; // no such comp

    comps_pool = avl_node<comp_id_t, comp_t>::remove(comps_pool, iter->value);
    comps_id_gen.free_id(id);
}

bool ram_driver::has_comp(comp_id_t id) noexcept {
    return avl_node<comp_id_t, comp_t>::count(comps_pool, id);
}

comp_type ram_driver::read_comp_type(comp_id_t id) noexcept {
    return comp_t::get_type(avl_node<comp_id_t, comp_t>::binary_search(comps_pool, id)->data);
}

void ram_driver::read_comp(comp_id_t id, comp_t *dest) noexcept {
    *dest = avl_node<comp_id_t, comp_t>::binary_search(comps_pool, id)->data;
}

void ram_driver::write_comp(const comp_t *comp) noexcept {
    auto iter = avl_node<comp_id_t, comp_t>::binary_search(comps_pool, comp_t::get_id(*comp));
    if (iter == nullptr) return; // the comp doesn't exist

    iter->data = *comp;
}


refs_id_t ram_driver::allocte_refs() noexcept {
    refs_id_t id = refs_id_gen.generate_id();
    refs_pool = avl_node<refs_id_t, refs_t>::insert(refs_pool, id, refs_t(id));
    return id;
}

void ram_driver::deallocate_refs(refs_id_t id) noexcept {
    auto iter = avl_node<refs_id_t, refs_t>::binary_search(refs_pool, id);
    if (iter == nullptr) return; // no such ref

    refs_pool = avl_node<refs_id_t, refs_t>::remove(refs_pool, iter->value);
    refs_id_gen.free_id(id);
}

bool ram_driver::has_refs(refs_id_t id) noexcept {
    return avl_node<refs_id_t, refs_t>::count(refs_pool, id);
}

void ram_driver::read_refs(refs_id_t id, refs_t *dest) noexcept {
    *dest = avl_node<refs_id_t, refs_t>::binary_search(refs_pool, id)->data;
}

void ram_driver::write_refs(const refs_t *refs) noexcept {
    auto iter = avl_node<refs_id_t, refs_t>::binary_search(refs_pool, refs->id);
    if (iter == nullptr) return; // no such ref

    iter->data = *refs;
}


content_id_t ram_driver::allocate_content() noexcept {
    content_id_t id = content_id_gen.generate_id();
    content_pool = avl_node<content_id_t, content_t>::insert(content_pool, id, content_t(null_content(id)));
    return id;
}

void ram_driver::deallocate_content(content_id_t id) noexcept {
    auto iter = avl_node<content_id_t, content_t>::binary_search(content_pool, id);
    if (iter == nullptr) return; // no such data

    content_pool = avl_node<content_id_t, content_t>::remove(content_pool, iter->value);
    content_id_gen.free_id(id);
}

bool ram_driver::has_content(content_id_t id) noexcept {
    return avl_node<content_id_t, content_t>::count(content_pool, id);
}

content_type ram_driver::read_content_type(content_id_t id) noexcept {
    return content_t::get_type(avl_node<content_id_t, content_t>::binary_search(content_pool, id)->data);
}

void ram_driver::read_content(content_id_t id, content_t *dest) noexcept {
    *dest = avl_node<content_id_t, content_t>::binary_search(content_pool, id)->data;
}

void ram_driver::write_content(const content_t *data) noexcept {
    auto iter = avl_node<content_id_t, content_t>::binary_search(content_pool, data->get_id(*data));
    if (iter == nullptr) return; // no such data

    iter->data = *data;
}
