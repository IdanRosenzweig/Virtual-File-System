#include "fs_ram_driver.h"

#include <iostream>
#include <memory>

node_id fs_ram_driver::allocte_node() {
    node_id id = nodes_id_gen.generate_id();
    nodes_pool[id] = node(id);
    return id;
}

void fs_ram_driver::free_node(node_id id) {
    auto iter = nodes_pool.find(id);
    if (iter == nodes_pool.end()) return; // no such node

    nodes_pool.erase(iter);
    nodes_id_gen.free_id(id);
}

bool fs_ram_driver::has_node(node_id id) {
    return nodes_pool.count(id);
}

std::unique_ptr<node> fs_ram_driver::read_node(node_id id) {
    // no need to actually read from the disk and allocate the node,
    // because the fs is allocated on the ram...
    const auto& iter =  nodes_pool[id];
    node read(id);
    if (true) read.name = iter.name;
    if (true) read.data_id = iter.data_id;
    if (true) read.refs_id = iter.refs_id;
    return std::make_unique<node>(std::move(read));
}

void fs_ram_driver::write_node(const node *node) {
    auto iter = nodes_pool.find(node->id);
    if (iter == nodes_pool.end()) return; // the node doesn't exist

    iter->second = *node;
}

node_data_id fs_ram_driver::allocate_node_data() {
    node_data_id id = data_id_gen.generate_id();
    data_pool[id] = node_data(null_data(id));
    return id;
}

void fs_ram_driver::free_node_data(node_data_id id) {
    auto iter = data_pool.find(id);
    if (iter == data_pool.end()) return; // no such data

    data_pool.erase(iter);
    data_id_gen.free_id(id);
}

bool fs_ram_driver::has_node_data(node_data_id id) {
    return data_pool.count(id);
}

node_data_type fs_ram_driver::read_node_data_type(node_data_id id) {
    return node_data::data_type(data_pool[id]);
}

std::unique_ptr<node_data> fs_ram_driver::read_node_data(node_data_id id) {
    // no need to actually read from the disk and allocate the data,
    // because the fs is allocated on the ram...
    const auto& iter = data_pool[id];
    return std::make_unique<node_data>(iter);
}

void fs_ram_driver::write_node_data(const node_data *data) {
    auto iter = data_pool.find(node_data::data_id(*data));
    if (iter == data_pool.end()) return; // the data doesn't exist

    iter->second = *data;
}

node_refs_id fs_ram_driver::allocte_node_refs() {
    node_refs_id id = refs_id_gen.generate_id();
    refs_pool[id] = node_refs(id);
    return id;
}

void fs_ram_driver::free_node_refs(node_refs_id id) {
    auto iter = refs_pool.find(id);
    if (iter == refs_pool.end()) return; // no such ref

    refs_pool.erase(iter);
    refs_id_gen.free_id(id);
}

bool fs_ram_driver::has_node_refs(node_refs_id id) {
    return refs_pool.count(id);
}

std::unique_ptr<node_refs>  fs_ram_driver::read_node_refs(node_refs_id id) {
    // no need to actually read from the disk and allocate the refs,
    // because the fs is allocated on the ram...
    const auto& iter = refs_pool[id];
    return std::make_unique<node_refs>(iter);
}

void fs_ram_driver::write_node_refs(const node_refs *refs) {
    auto iter = refs_pool.find(refs->id);
    if (iter == refs_pool.end()) return; // the data doesn't exist

    iter->second = *refs;
}
