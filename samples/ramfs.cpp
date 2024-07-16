#include "ramfs.h"

#include <iostream>
#include <memory>
using namespace std;

node_id ramfs::allocte_node() {
    node_id id = nodes_id_gen.generate_id();
    nodes_pool[id] = node(id);
    return id;
}

void ramfs::free_node(node_id id) {
    auto iter = nodes_pool.find(id);
    if (iter == nodes_pool.end()) return; // no such node

    nodes_pool.erase(iter);
    nodes_id_gen.free_id(id);
}

bool ramfs::has_node(node_id id) {
    return nodes_pool.count(id);
}

loaded_node ramfs::read_node(node_id id, bool node_name, bool node_data) {
    // no need to actually read from the disk and allocate the node,
    // because the fs is allocated on the ram...
    const auto& iter =  nodes_pool[id];
    node read(id);
    if (node_name) read.name = iter.name;
    if (node_data) read.data_id = iter.data_id;
    return loaded_node(this, std::make_unique<node>(std::move(read)));
}

void ramfs::update_node(const node *node) {
    auto iter = nodes_pool.find(node->id);
    if (iter == nodes_pool.end()) return; // the node doesn't exist

    iter->second = *node;
}

node_data_id ramfs::allocate_node_data() {
    node_data_id id = data_id_gen.generate_id();
    data_pool[id] = node_data(id);
    return id;
}

void ramfs::free_node_data(node_data_id id) {
    auto iter = data_pool.find(id);
    if (iter == data_pool.end()) return; // no such node

    data_pool.erase(iter);
    data_id_gen.free_id(id);
}

bool ramfs::has_node_data(node_data_id id) {
    return data_pool.count(id);
}

std::unique_ptr<node_data> ramfs::read_node_data(node_data_id id) {
    // no need to actually read from the disk and allocate the data,
    // because the fs is allocated on the ram...
    const auto& iter = data_pool[id];
    return std::make_unique<node_data>(iter);
}

void ramfs::update_node_data(const node_data *data) {
    auto iter = data_pool.find(data->id);
    if (iter == data_pool.end()) return; // the data doesn't exist

    iter->second = *data;
}

node_id ramfs::get_root_node_id() {
    return root_id;
}
