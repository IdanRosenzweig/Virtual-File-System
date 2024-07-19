#include "dir.h"

#include "../../basic_hierarchy.h"

void dir::add_child_to_dir(basic_hierarchy *fs, dir *dir, comp_id_t id, bool add_to_refs) {
    if (dir->children.count(id)) return;

    dir->children.insert(id);

    std::unique_ptr<node> node(fs->stat_node(id));
    trie::add_string(&dir->trie_root, node->name.data(), node->name.size())->data = id;

    if (add_to_refs) {
        auto refs = fs->driver->read_refs(id);
        refs->dirs.insert(dir->id);
        fs->driver->write_refs(refs.get());
    }
}

void dir::remove_child_from_dir(basic_hierarchy *fs, dir *dir, comp_id_t id, bool remove_from_refs) {
    if (!dir->children.count(id)) return;

    dir->children.erase(id);
    std::unique_ptr<node> node(fs->stat_node(id));
    trie::remove_string(&dir->trie_root, node->name.data(), node->name.size());

    if (remove_from_refs) {
        auto refs = fs->driver->read_refs(fs->driver->read_node(id)->refs_id);
        refs->dirs.erase(dir->id);
        fs->driver->write_refs(refs.get());
    }
}

bool dir::has_child(basic_hierarchy *fs, dir *dir, comp_id_t id) {
    return dir->children.count(id);
}

std::vector<comp_id_t> dir::get_all_children(basic_hierarchy *fs, dir *dir) {
    std::vector<comp_id_t> res;
    for (const comp_id_t &id: dir->children)
        res.push_back(id);
    return res;
}

comp_id_t dir::search_path_comp(basic_hierarchy *fs, dir *dir, const path_comp &comp) {
    trie *trie_node = trie::search_string(&dir->trie_root, comp.data(), comp.size());
    if (trie_node == nullptr) return comp_id_null;
    else return trie_node->data;
}
