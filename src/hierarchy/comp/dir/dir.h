#ifndef DIR_H
#define DIR_H

#include <cstdint>
#include <vector>
#include <set>
#include <string>

#include "lib/trie.h"

#include "../base_comp.h"
#include "src/hierarchy/path/path.h"

struct basic_hierarchy;

// a node that points to multiple child nodes
struct dir : public base_comp {
    // the actual children stored in this dir
    std::set<comp_id_t> children;

    // a trie to associate strings with children
    static constexpr auto TRIE_ASSIGN = [](const uint8_t *val) -> int { return *val - 'a'; };
    typedef trie_node<uint8_t, comp_id_t, 'z' - 'a' + 1, TRIE_ASSIGN> trie;
    trie trie_root;

    dir() = default;

    explicit dir(comp_id_t id)
        : base_comp(id) {
    }

    dir(const dir &other)
        : base_comp(other),
          children(other.children),
          trie_root(other.trie_root) {
    }

    dir(dir &&other) noexcept
        : base_comp(std::move(other)),
          children(std::move(other.children)),
          trie_root(std::move(other.trie_root)) {
    }

    dir &operator=(const dir &other) {
        if (this == &other)
            return *this;
        base_comp::operator =(other);
        children = other.children;
        trie_root = other.trie_root;
        return *this;
    }

    dir &operator=(dir &&other) noexcept {
        if (this == &other)
            return *this;
        base_comp::operator =(std::move(other));
        children = std::move(other.children);
        trie_root = std::move(other.trie_root);
        return *this;
    }

    static void add_child_to_dir(basic_hierarchy *fs, dir *dir, comp_id_t id, bool add_to_refs);

    static void remove_child_from_dir(basic_hierarchy *fs, dir *dir, comp_id_t id, bool remove_from_refs);

    static bool has_child(basic_hierarchy *fs, dir *dir, comp_id_t id);

    static std::vector<comp_id_t> get_all_children(basic_hierarchy *fs, dir *dir);

    static comp_id_t search_path_comp(basic_hierarchy *fs, dir *dir, const path_comp &comp);
};


#endif //DIR_H
