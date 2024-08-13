#include "dir.h"

#include "src/hierarchy/hierarchy.h"
#include "../comp_t.h"

void dir::add_child_to_dir(ctx_t<comp_t> &dir_comp, ctx_t<node_id_t> child, bool add_to_refs) {
    dir *dir_ptr = comp_t::get_ptr<dir>(&dir_comp.val);

    if (avl::count(dir_ptr->children, child)) return;

    // insert the child into the set
    avl::insert(dir_ptr->children, child, nullptr);

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
        refs_t::avl::insert(refs.val.dirs, {dir_comp.hier, dir_ptr->id}, nullptr);
        refs.hier->driver->write_refs(&refs.val); // write back to driver
    }
}

void dir::remove_child_from_dir(ctx_t<comp_t> &dir_comp, ctx_t<node_id_t> child, bool remove_from_refs) {
    dir *dir_ptr = comp_t::get_ptr<dir>(&dir_comp.val);

    if (avl::count(dir_ptr->children, child)) return;

    // remove the child from the set
    avl::remove(dir_ptr->children, child);

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
        refs_t::avl::remove(refs.val.dirs, {dir_comp.hier, dir_ptr->id});
        refs.hier->driver->write_refs(&refs.val); // write back to driver
    }
}

bool dir::has_child(comp_t *dir_comp, ctx_t<node_id_t> child) {
    dir *dir_ptr = comp_t::get_ptr<dir>(dir_comp);
    return avl::count(dir_ptr->children, child);
}

::vector<ctx_t<node_id_t> > dir::get_all_children(comp_t *dir_comp) {
    dir *dir_ptr = comp_t::get_ptr<dir>(dir_comp);

    ::vector<ctx_t<node_id_t> > res;
    avl* it = avl::begin(dir_ptr->children);
    while (it != nullptr) {
        res.push_back(it->value);
        it = avl::next(it);
    }
    return res;
}

ctx_t<node_id_t> dir::search_path_comp(comp_t *dir_comp, const path_comp &comp) {
    dir *dir_ptr = comp_t::get_ptr<dir>(dir_comp);

    trie *trie_node = trie::search_string(&dir_ptr->trie_root, comp.data(), comp.size());
    if (trie_node == nullptr) return null_ctx<node_id_t>();
    else return trie_node->data;
}
