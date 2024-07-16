#include "dir.h"

#include "src/fs/fs.h"

void dir::add_child_to_dir(dir *dir, node_id id) {
    if (dir->children.count(id)) return;

    dir->children.insert(id);
    std::unique_ptr<node> node(dir->owner_fs->stat_node(id));
    trie::add_string(&dir->trie_root, node->name.data(), node->name.size())->data = id;
}

void dir::remove_child_from_dir(dir *dir, node_id id) {
    if (!dir->children.count(id)) return;

    dir->children.erase(id);
    std::unique_ptr<node> node(dir->owner_fs->stat_node(id));
    trie::remove_string(&dir->trie_root, node->name.data(), node->name.size());
}

bool dir::has_child(dir *dir, node_id id) {
    return dir->children.count(id);
}

std::vector<node_id> dir::get_all_children(dir *dir) {
    std::vector<node_id> res;
    for (const node_id &id: dir->children)
        res.push_back(id);
    return res;
}

node_id dir::search_path_comp(dir *dir, const path_comp &comp) {
    trie *trie_node = trie::search_string(&dir->trie_root, comp.data(), comp.size());
    if (trie_node == nullptr) return node_id_null;
    else return trie_node->data;
}
