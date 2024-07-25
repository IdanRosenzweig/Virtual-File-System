#include "ram_driver.h"

#include <iostream>
#include <memory>

node_id_t ram_driver::allocte_node() noexcept {
    node_id_t id = nodes_id_gen.generate_id();
    nodes_pool[id] = node(id);
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

std::unique_ptr<node> ram_driver::read_node(node_id_t id) noexcept {
    // no need to actually read from the disk and allocate the node,
    // because the DATA is allocated on the ram...
    const auto& iter =  nodes_pool[id];
    node read(id);
    if (true) read.name = iter.name;
    if (true) read.comp_id = iter.comp_id;
    if (true) read.refs_id = iter.refs_id;
    if (true) read.content_id = iter.content_id;
    return std::make_unique<node>(std::move(read));
}

void ram_driver::write_node(const node *node) noexcept {
    auto iter = nodes_pool.find(node->id);
    if (iter == nodes_pool.end()) return; // the node doesn't exist

    iter->second = *node;
}


comp_id_t ram_driver::allocate_comp() noexcept {
    comp_id_t id = comps_id_gen.generate_id();
    comps_pool[id] = comp(null_comp(id));
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
    return comp::get_type(comps_pool[id]);
}

std::unique_ptr<comp> ram_driver::read_comp(comp_id_t id) noexcept {
    // no need to actually read from the disk and allocate the comp,
    // because the DATA is allocated on the ram...
    const auto& iter = comps_pool[id];
    return std::make_unique<comp>(iter);
}

void ram_driver::write_comp(const comp *comp) noexcept {
    auto iter = comps_pool.find(comp::get_id(*comp));
    if (iter == comps_pool.end()) return; // the comp doesn't exist

    iter->second = *comp;
}


refs_id_t ram_driver::allocte_refs() noexcept {
    refs_id_t id = refs_id_gen.generate_id();
    refs_pool[id] = refs(id);
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

std::unique_ptr<refs>  ram_driver::read_refs(refs_id_t id) noexcept {
    // no need to actually read from the disk and allocate the refs,
    // because the DATA is allocated on the ram...
    const auto& iter = refs_pool[id];
    return std::make_unique<refs>(iter);
}

void ram_driver::write_refs(const refs *refs) noexcept {
    auto iter = refs_pool.find(refs->id);
    if (iter == refs_pool.end()) return; // the data doesn't exist

    iter->second = *refs;
}


content_id_t ram_driver::allocate_content() noexcept {
    content_id_t id = content_id_gen.generate_id();
    content_pool[id] = content(null_content(id));
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
    return content::get_type(content_pool[id]);
}

std::unique_ptr<content> ram_driver::read_content(content_id_t id) noexcept {
    // no need to actually read from the disk and allocate the data,
    // because the fs is allocated on the ram...
    const auto& iter = content_pool[id];
    return std::make_unique<content>(iter);
}

void ram_driver::write_content(const content *data) noexcept {
    auto iter = content_pool.find(content::get_id(*data));
    if (iter == content_pool.end()) return; // the data doesn't exist

    iter->second = *data;
}