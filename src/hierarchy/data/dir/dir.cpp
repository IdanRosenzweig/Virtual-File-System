#include "dir.h"

#include "src/fs/fs.h"

void dir::add_child_to_dir(fs *fs, dir *dir, node_id id, bool add_to_refs) {
    if (dir->children.count(id)) return;

    dir->children.insert(id);

    loaded_node node(fs->stat_node(id));
    trie::add_string(&dir->trie_root, node.ptr->name.data(), node.ptr->name.size())->data = id;

    if (add_to_refs) {
        auto refs = fs->read_node_refs(id);
        refs->dirs.insert(dir->id);
        fs->write_node_refs(refs.get());
    }
}

void dir::remove_child_from_dir(fs *fs, dir *dir, node_id id, bool remove_from_refs) {
    if (!dir->children.count(id)) return;

    dir->children.erase(id);
    loaded_node node(fs->stat_node(id));
    trie::remove_string(&dir->trie_root, node.ptr->name.data(), node.ptr->name.size());

    if (remove_from_refs) {
        auto refs = fs->read_node_refs(fs->read_node(id).ptr->refs_id);
        refs->dirs.erase(dir->id);
        fs->write_node_refs(refs.get());
    }
}

bool dir::has_child(fs *fs, dir *dir, node_id id) {
    return dir->children.count(id);
}

std::vector<node_id> dir::get_all_children(fs *fs, dir *dir) {
    std::vector<node_id> res;
    for (const node_id &id: dir->children)
        res.push_back(id);
    return res;
}

node_id dir::search_path_comp(fs *fs, dir *dir, const path_comp &comp) {
    trie *trie_node = trie::search_string(&dir->trie_root, comp.data(), comp.size());
    if (trie_node == nullptr) return node_id_null;
    else return trie_node->data;
}
