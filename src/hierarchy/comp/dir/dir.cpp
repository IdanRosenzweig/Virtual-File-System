#include "dir.h"

#include "../../hierarchy.h"
#include "../comp.h"

void dir::add_child_to_dir(const ctx_t<std::unique_ptr<comp>>& dir_comp, ctx_t<node_id_t> child, bool add_to_refs) {
    dir* dir_ptr = comp::get_ptr<dir>(dir_comp.val.get());

    if (dir_ptr->children.count(child)) return;

    // insert the child into the set
    dir_ptr->children.insert(child);

    // insert the child name into the trie
    std::unique_ptr<node> node(hierarchy::stat_node(child).val); // reading to get the node name
    trie::add_string(&dir_ptr->trie_root, node->name.data(), node->name.size())->data = child;

    // write the directory to driver
    dir_comp.hier->driver->write_comp(dir_comp.val.get());

    if (add_to_refs) { // add a reference to the child node
        auto refs = hierarchy::read_refs(hierarchy::stat_refs_id(child));
        refs.val->dirs.insert({dir_comp.hier, dir_ptr->id});
        refs.hier->driver->write_refs(refs.val.get()); // write back to driver
    }
}

void dir::remove_child_from_dir(const ctx_t<std::unique_ptr<comp>>& dir_comp, ctx_t<node_id_t> child, bool remove_from_refs) {
    dir* dir_ptr = comp::get_ptr<dir>(dir_comp.val.get());

    if (dir_ptr->children.count(child)) return;

    // remove the child from the set
    dir_ptr->children.erase(child);

    // remove the child name from the trie
    std::unique_ptr<node> node(hierarchy::stat_node(child).val); // reading to get the node name
    trie::remove_string(&dir_ptr->trie_root, node->name.data(), node->name.size());

    // write the directory to driver
    dir_comp.hier->driver->write_comp(dir_comp.val.get());


    if (remove_from_refs) { // remove the reference from child
        auto refs = hierarchy::read_refs(hierarchy::stat_refs_id(child));
        refs.val->dirs.erase({dir_comp.hier, dir_ptr->id});
        refs.hier->driver->write_refs(refs.val.get()); // write back to driver
    }
}

bool dir::has_child(const ctx_t<std::unique_ptr<comp>> &dir_comp, ctx_t<node_id_t> child) {
    dir* dir_ptr = comp::get_ptr<dir>(dir_comp.val.get());
    return dir_ptr->children.count(child);
}

std::vector<ctx_t<node_id_t>> dir::get_all_children(const ctx_t<std::unique_ptr<comp>> &dir_comp) {
    dir* dir_ptr = comp::get_ptr<dir>(dir_comp.val.get());

    std::vector<ctx_t<node_id_t>> res;
    for (const ctx_t<node_id_t> &child: dir_ptr->children) res.push_back(child);
    return res;
}

ctx_t<node_id_t> dir::search_path_comp(const ctx_t<std::unique_ptr<comp>>& dir_comp, const path_comp &comp) {
    dir* dir_ptr = comp::get_ptr<dir>(dir_comp.val.get());

    trie *trie_node = trie::search_string(&dir_ptr->trie_root, comp.data(), comp.size());
    if (trie_node == nullptr) return null_ctx<node_id_t>();
    else return trie_node->data;
}
