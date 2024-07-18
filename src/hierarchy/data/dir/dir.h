#ifndef DIR_H
#define DIR_H

#include <cstdint>
#include <vector>
#include <set>
#include <string>

#include "lib/trie.h"

#include "../base_data.h"
#include "src/hierarchy/node/node_id.h"
#include "src/hierarchy/path/path.h"

// a node that points to multiple child nodes
struct dir : public base_data {
    // the actual children stored in this dir
    std::set<node_id> children;

    // a trie to associate strings with children
    static constexpr auto TRIE_ASSIGN = [](const uint8_t *val) -> int { return *val - 'a'; };
    typedef trie_node<uint8_t, node_id, 'z' - 'a' + 1, TRIE_ASSIGN> trie;
    trie trie_root;

    dir() : base_data() {
    }

    dir(const dir &other)
        : base_data(other),
          children(other.children),
          trie_root(other.trie_root) {
    }

    dir(dir &&other) noexcept
        : base_data(std::move(other)),
          children(std::move(other.children)),
          trie_root(std::move(other.trie_root)) {
    }

    dir &operator=(const dir &other) {
        if (this == &other)
            return *this;
        base_data::operator =(other);
        children = other.children;
        trie_root = other.trie_root;
        return *this;
    }

    dir &operator=(dir &&other) noexcept {
        if (this == &other)
            return *this;
        base_data::operator =(std::move(other));
        children = std::move(other.children);
        trie_root = std::move(other.trie_root);
        return *this;
    }

    static void add_child_to_dir(fs *fs, dir *dir, node_id id, bool add_to_refs);

    static void remove_child_from_dir(fs *fs, dir *dir, node_id id, bool remove_from_refs);

    static bool has_child(fs *fs, dir *dir, node_id id);

    static std::vector<node_id> get_all_children(fs *fs, dir *dir);

    static node_id search_path_comp(fs *fs, dir *dir, const path_comp &comp);
};


#endif //DIR_H
