#include "dir.h"

#include "../../hierarchy.h"
#include "../comp_t.h"

void dir::add_child_to_dir(ctx_t<comp_t> &dir_comp, ctx_t<node_id_t> child, bool add_to_refs) {
    dir *dir_ptr = comp_t::get_ptr<dir>(&dir_comp.val);

    if (dir_ptr->children.count(child)) return;

    // insert the child into the set
    dir_ptr->children.insert(child);

    // insert the child name into the trie
    ctx_t<node_t> child_node;
    hierarchy::stat_node(child, &child_node);
    trie::add_string(&dir_ptr->trie_root, child_node.val.name.data(), child_node.val.name.size())->data = child;

    // write the directory to driver
    dir_comp.hier->driver->write_comp(&dir_comp.val);

    if (add_to_refs) {
        // add a reference to the child node
        ctx_t<refs_t> refs;
        hierarchy::stat_refs(hierarchy::stat_refs_id(child), &refs);
        refs.val.dirs.insert({dir_comp.hier, dir_ptr->id});
        refs.hier->driver->write_refs(&refs.val); // write back to driver
    }
}

void dir::remove_child_from_dir(ctx_t<comp_t> &dir_comp, ctx_t<node_id_t> child, bool remove_from_refs) {
    dir *dir_ptr = comp_t::get_ptr<dir>(&dir_comp.val);

    if (dir_ptr->children.count(child)) return;

    // remove the child from the set
    dir_ptr->children.erase(child);

    // remove the child name from the trie
    ctx_t<node_t> child_node;
    hierarchy::stat_node(child, &child_node);
    trie::remove_string(&dir_ptr->trie_root, child_node.val.name.data(), child_node.val.name.size());

    // write the directory to driver
    dir_comp.hier->driver->write_comp(&dir_comp.val);


    if (remove_from_refs) {
        // remove the reference from child
        ctx_t<refs_t> refs;
        hierarchy::stat_refs(hierarchy::stat_refs_id(child), &refs);
        refs.val.dirs.erase({dir_comp.hier, dir_ptr->id});
        refs.hier->driver->write_refs(&refs.val); // write back to driver
    }
}

bool dir::has_child(comp_t *dir_comp, ctx_t<node_id_t> child) {
    dir *dir_ptr = comp_t::get_ptr<dir>(dir_comp);
    return dir_ptr->children.count(child);
}

std::vector<ctx_t<node_id_t> > dir::get_all_children(comp_t *dir_comp) {
    dir *dir_ptr = comp_t::get_ptr<dir>(dir_comp);

    std::vector<ctx_t<node_id_t> > res;
    for (const ctx_t<node_id_t> &child: dir_ptr->children) res.push_back(child);
    return res;
}

ctx_t<node_id_t> dir::search_path_comp(comp_t *dir_comp, const path_comp &comp) {
    dir *dir_ptr = comp_t::get_ptr<dir>(dir_comp);

    trie *trie_node = trie::search_string(&dir_ptr->trie_root, comp.data(), comp.size());
    if (trie_node == nullptr) return null_ctx<node_id_t>();
    else return trie_node->data;
}
