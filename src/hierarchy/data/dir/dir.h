#ifndef DIR_H
#define DIR_H

#include <cstdint>
#include <vector>
#include <set>
#include <string>

#include "lib/trie.h"

#include "src/hierarchy/base/node/node_id.h"
#include "src/hierarchy/base/path/path.h"

struct fs;

// a node that points to multiple child nodes
struct dir {
private:
    // node_id base_node;

    // the actual children stored in this dir
    std::set<node_id> children;

public:
    dir() {}

    dir(const dir &other)
        : children(other.children),
          trie_root(other.trie_root) {
    }

    dir(dir &&other) noexcept
        : children(std::move(other.children)),
          trie_root(std::move(other.trie_root)) {
    }

    dir & operator=(const dir &other) {
        if (this == &other)
            return *this;
        children = other.children;
        trie_root = other.trie_root;
        return *this;
    }

    dir & operator=(dir &&other) noexcept {
        if (this == &other)
            return *this;
        children = std::move(other.children);
        trie_root = std::move(other.trie_root);
        return *this;
    }

    // dir(node_id base_node)
    //     : base_node(base_node) {
    // }
    //
    // dir(const dir &other)
    //     : base_node(other.base_node),
    //       children(other.children),
    //       trie_root(other.trie_root) {
    // }
    //
    // dir(dir &&other) noexcept
    //     : base_node(other.base_node),
    //       children(std::move(other.children)),
    //       trie_root(std::move(other.trie_root)) {
    // }
    //
    // dir & operator=(const dir &other) {
    //     if (this == &other)
    //         return *this;
    //     base_node = other.base_node;
    //     children = other.children;
    //     trie_root = other.trie_root;
    //     return *this;
    // }
    //
    // dir & operator=(dir &&other) noexcept {
    //     if (this == &other)
    //         return *this;
    //     base_node = other.base_node;
    //     children = std::move(other.children);
    //     trie_root = std::move(other.trie_root);
    //     return *this;
    // }

    static void add_child_to_dir(fs *fs, dir *dir, node_id id);

    static void remove_child_from_dir(fs*fs,dir *dir, node_id id);

    static bool has_child(fs*fs,dir *dir, node_id id);

    static std::vector<node_id> get_all_children(fs*fs,dir *dir);

    static node_id search_path_comp(fs*fs,dir *dir, const path_comp &comp);

private:
    // a trie to associate strings with children
    static constexpr auto TRIE_ASSIGN = [](const uint8_t *val) -> int { return *val - 'a'; };
    typedef trie_node<uint8_t, node_id, 'z' - 'a' + 1, TRIE_ASSIGN> trie;
    trie trie_root;

public:
    static void associate_string_with_child(dir *dir, node_id, const std::string &str);

    static void disassociate_string_with_child(dir *dir, node_id, const std::string &str);

    static node_id search_child(dir *dir, const std::string &str);
};


#endif //DIR_H
