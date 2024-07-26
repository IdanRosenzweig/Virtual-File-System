#ifndef DIR_H
#define DIR_H

#include <cstdint>
#include <vector>
#include <set>
#include <string>
#include <memory>

#include "lib/trie.h"

#include "../base_comp.h"
#include "src/hierarchy/ctx_t.h"
#include "src/hierarchy/node/node_id.h"
#include "src/hierarchy/path/path.h"

struct comp_t;

// a node that points to multiple child nodes
struct dir : public base_comp {
    // the actual children stored in this dir
    std::set<ctx_t<node_id_t> > children;

    // a trie to associate strings with children
    static constexpr auto TRIE_ASSIGN = [](const uint8_t *val) -> int { return *val - 'a'; };
    typedef trie_node<uint8_t, ctx_t<node_id_t>, 'z' - 'a' + 1, TRIE_ASSIGN> trie;
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

    static void add_child_to_dir(ctx_t<comp_t> &dir_comp, ctx_t<node_id_t> child, bool add_to_refs);

    static void remove_child_from_dir(ctx_t<comp_t> &dir_comp, ctx_t<node_id_t> child, bool remove_from_refs);

    static bool has_child(comp_t *dir_comp, ctx_t<node_id_t> child);

    static std::vector<ctx_t<node_id_t> > get_all_children(comp_t *dir_comp);

    static ctx_t<node_id_t> search_path_comp(comp_t *dir_comp, const path_comp &comp);
};


#endif //DIR_H
