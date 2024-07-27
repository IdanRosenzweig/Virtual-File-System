#include "ram_driver.h"

#include <iostream>
#include <memory>

node_id_t ram_driver::allocte_node() noexcept {
    node_id_t id = nodes_id_gen.generate_id();
    nodes_pool[id] = node_t(id);
    return id;
}

void ram_driver::deallocate_node(node_id_t id) noexcept {
    auto iter = nodes_pool.find(id);
    if (iter == nodes_pool.end()) return; // no such node

    nodes_pool.erase(iter);
    nodes_id_gen.free_id(id);
}

bool ram_driver::has_node(node_id_t id) noexcept {
    return nodes_pool.count(id);
}

void ram_driver::read_node(node_id_t id, node_t *dest) noexcept {
    *dest = nodes_pool[id];
}

void ram_driver::write_node(const node_t *node) noexcept {
    auto iter = nodes_pool.find(node->id);
    if (iter == nodes_pool.end()) return; // the node doesn't exist

    iter->second = *node;
}


comp_id_t ram_driver::allocate_comp() noexcept {
    comp_id_t id = comps_id_gen.generate_id();
    comps_pool[id] = comp_t(comp_data(null_comp(id)));
    return id;
}

void ram_driver::deallocate_comp(comp_id_t id) noexcept {
    auto iter = comps_pool.find(id);
    if (iter == comps_pool.end()) return; // no such comp

    comps_pool.erase(iter);
    comps_id_gen.free_id(id);
}

bool ram_driver::has_comp(comp_id_t id) noexcept {
    return comps_pool.count(id);
}

comp_type ram_driver::read_comp_type(comp_id_t id) noexcept {
    return comp_t::get_type(comps_pool[id]);
}

void ram_driver::read_comp(comp_id_t id, comp_t *dest) noexcept {
    *dest = comps_pool[id];
}

void ram_driver::write_comp(const comp_t *comp) noexcept {
    auto iter = comps_pool.find(comp_t::get_id(*comp));
    if (iter == comps_pool.end()) return; // the comp doesn't exist

    iter->second = *comp;
}


refs_id_t ram_driver::allocte_refs() noexcept {
    refs_id_t id = refs_id_gen.generate_id();
    refs_pool[id] = refs_t(id);
    return id;
}

void ram_driver::deallocate_refs(refs_id_t id) noexcept {
    auto iter = refs_pool.find(id);
    if (iter == refs_pool.end()) return; // no such ref

    refs_pool.erase(iter);
    refs_id_gen.free_id(id);
}

bool ram_driver::has_refs(refs_id_t id) noexcept {
    return refs_pool.count(id);
}

void ram_driver::read_refs(refs_id_t id, refs_t *dest) noexcept {
    *dest = refs_pool[id];
}

void ram_driver::write_refs(const refs_t *refs) noexcept {
    auto iter = refs_pool.find(refs->id);
    if (iter == refs_pool.end()) return; // the data doesn't exist

    iter->second = *refs;
}


content_id_t ram_driver::allocate_content() noexcept {
    content_id_t id = content_id_gen.generate_id();
    content_pool[id] = content_t(null_content(id));
    return id;
}

void ram_driver::deallocate_content(content_id_t id) noexcept {
    auto iter = content_pool.find(id);
    if (iter == content_pool.end()) return; // no such data

    content_pool.erase(iter);
    content_id_gen.free_id(id);
}

bool ram_driver::has_content(content_id_t id) noexcept {
    return content_pool.count(id);
}

content_type ram_driver::read_content_type(content_id_t id) noexcept {
    return content_t::get_type(content_pool[id]);
}

void ram_driver::read_content(content_id_t id, content_t *dest) noexcept {
    *dest = content_pool[id];
}

void ram_driver::write_content(const content_t *data) noexcept {
    auto iter = content_pool.find(content_t::get_id(*data));
    if (iter == content_pool.end()) return; // the data doesn't exist

    iter->second = *data;
}
