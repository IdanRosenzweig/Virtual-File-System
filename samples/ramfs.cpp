#include "ramfs.h"

#include <iostream>
#include <memory>
using namespace std;

node_id ramfs::allocte_node() {
    node_id id = id_gen.generate_id();
    nodes[id] = node({this, id});
    return id;
}

void ramfs::free_node(node_id id) {
    auto iter = nodes.find(id);
    if (iter == nodes.end()) return; // no such node

    nodes.erase(iter);
    id_gen.free_id(id);
}

bool ramfs::has_node(node_id id) {
    return nodes.count(id);
}

std::unique_ptr<node> ramfs::read_node(node_id id) {
    // no need to actually read from the disk and allocate the node,
    // because the fs is allocated on the ram...
    const auto& iter =  nodes[id];
    return std::make_unique<node>(iter);
}

void ramfs::update_node(const node *node) {
    auto iter = nodes.find(node->fs_related.id);
    if (iter == nodes.end()) return; // the node doesn't exist

    iter->second = *node;
}

node_id ramfs::get_root_node_id() {
    return root_id;
}
