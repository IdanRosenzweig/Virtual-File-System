#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <queue>
#include <algorithm>

#define PARENT_PTR 1
#define VAL_AT_NODE 1
#define CUSTOM_DATA_AT_NODE 1
#define DEALLOCTE_MEM 1

template<
    typename TYPE
#if CUSTOM_DATA_AT_NODE == 1
    , typename DATA
#endif
    , int RANGE // max number of children
    , int (*ASSIGN_INDEX)(const TYPE *val) // function that assigns indexes in the range [0, RANGE) to values
>
struct trie_node {
protected:
    // internal trie data
    trie_node *children[RANGE] = {nullptr};
    int child_count = 0;
#if PARENT_PTR == 1
    trie_node *parent = nullptr;
#endif

public:
    bool marked = false;
#if VAL_AT_NODE == 1
    TYPE val;
#endif
#if CUSTOM_DATA_AT_NODE == 1
    DATA data; // additional custom node data
#endif

#if DEALLOCTE_MEM == 1
    virtual ~trie_node() {
        remove_all_children(this);
    }
#endif

    trie_node() {
    }

    trie_node(const trie_node &other)
        : child_count(other.child_count),
          parent(other.parent),
          marked(other.marked),
          val(other.val),
          data(other.data) {
        for (int i = 0; i < RANGE; i++) {
            if (other.children[i] != nullptr) {
                children[i] = new trie_node(*other.children[i]);
                children[i]->parent = this;
            }
        }
    }

    trie_node(trie_node &&other) noexcept
        : child_count(other.child_count),
          parent(other.parent),
          marked(other.marked),
          val(std::move(other.val)),
          data(std::move(other.data)) {
        for (int i = 0; i < RANGE; i++) {
            children[i] = other.children[i];
            other.children[i] = nullptr;
        }
    }

    trie_node & operator=(const trie_node &other) {
        if (this == &other)
            return *this;
        child_count = other.child_count;
        parent = other.parent;
        marked = other.marked;
        val = other.val;
        data = other.data;
        for (int i = 0; i < RANGE; i++) {
            if (other.children[i] != nullptr) {
                children[i] = new trie_node(*other.children[i]);
                children[i]->parent = this;
            }
        }
        return *this;
    }

    trie_node & operator=(trie_node &&other) noexcept {
        if (this == &other)
            return *this;
        child_count = other.child_count;
        parent = other.parent;
        marked = other.marked;
        val = std::move(other.val);
        data = std::move(other.data);
        for (int i = 0; i < RANGE; i++) {
            children[i] = other.children[i];
            other.children[i] = nullptr;
        }
        return *this;
    }

    /** the minimal trie function */

    static inline int number_of_children(trie_node *node) {
        return node == nullptr ? 0 : node->child_count;
    }

    // go downwards from the node and follow the value.
    // return pointer to the resulting node (or nullptr if doesn't exist)
    static inline trie_node *search_child(trie_node *node, const TYPE *val) {
        if (node == nullptr) return nullptr;
        return node->children[ASSIGN_INDEX(val)];
    }

    static inline bool has_child(trie_node* node, const TYPE* val) {
        return node == nullptr ? false : node->child_count[ASSIGN_INDEX(val)] != nullptr;
    }

    // go downwards from the node and allocate the child if it doesn't exist already.
    // if mark is true, the new child will be marked.
    // return pointer to the resulting node
    static inline trie_node *add_child(trie_node *node, const TYPE *val, bool mark = true) {
        if (node == nullptr) return nullptr;

        auto& child = node->children[ASSIGN_INDEX(val)];

        if (child == nullptr) {
            child = new trie_node;
#if PARENT_PTR == 1
            child->parent = node;
#endif
#if VAL_AT_NODE == 1
            child->val = *val;
#endif
            ++node->child_count;
        } // else // child already exists

        if (mark) child->marked = true;

        return child;
    }

    // search the child and (if found) remove it
    static inline void remove_child(trie_node *node, const TYPE *val) {
        if (node == nullptr) return;

        auto& child = node->children[ASSIGN_INDEX(val)];

        if (child == nullptr) return; // no such child

#if DEALLOCTE_MEM == 1
        delete child; // deconstructor will call child->remove_all_children()
#endif
        child = nullptr;
        --node->child_count;
    }

    // remove all the children from the node downwards
    static inline void remove_all_children(trie_node *node) {
        if (node == nullptr) return;

        for (int i = 0; i < RANGE; i++) {
            trie_node *child = node->children[i];
            if (child == nullptr) continue; // child doesn't exists

#if DEALLOCTE_MEM == 1
            delete child; // deconstructor will recursively call remove_all_children() on the child
#endif
            node->children[i] = nullptr;
        }
    }

    /** additional helper functions */

    // go downwards from the node and follow the string.
    // return pointer to the resulting node (or nullptr if doesn't exist)
    static trie_node *search_string(trie_node *node, const TYPE *str, int len) {
        int i = 0;
        while (node != nullptr && i++ < len) // either whole string was searched down, or the string doesn't exist and the node is nullptr
            node = search_child(node, str++);

        return node;
    }

    // go downwards from the node and add the string if it doesn't exist already.
    // if mark_end is true, the end node representing the string will be marked.
    // if mark_prefixes is true, all nodes along the string path (excluding the end node) will be marked.
    // return pointer to the resulting node
    static trie_node *add_string(trie_node *node, const TYPE *str, int len, bool mark_end = true,
                                 bool mark_prefixes = false) {
        if (node == nullptr) return nullptr;

        int i = 0;
        while (i++ < len) {
            node = add_child(node, str++, false); // create the child (if not exists already) and advance curr_node to it
            if ((i < len && mark_prefixes) || (i == len && mark_end)) node->marked = true;
        }

        return node;
    }

    // search the string going downward from node and (if found) remove its corresponding nodes
    static void remove_string(trie_node *node, const TYPE *str, int len) {
        if (node == nullptr) return;

        // through the path of the string, there can be nodes which correspond to other strings as well, not only this string.
        // we can remove the nodes which correspond only to the string, and not any other string.

        // the lowest_needed_ancestor pointer stores the lowest ancestor which is still needed by other strings
        trie_node *lowest_needed_ancestor = nullptr;
        // the val_in_ansector stores the value corresponding to the continuation of our string downwards from the ancestor
        TYPE val_in_ansector;

        int i = 0;
        while (i++ < len) {
            const TYPE *val = str++;

            if (i == 1 || node->marked || number_of_children(node) > 1) { // this node corresponds to other string besides ours (or it is the root of the trie)
                lowest_needed_ancestor = node;
                val_in_ansector = *val;
            }

            node = search_child(node, val);
            if (node == nullptr) return; // string doesn't even exists
        }
        if (!node->marked) return; // string is not marked

        if (lowest_needed_ancestor != nullptr) remove_child(lowest_needed_ancestor, &val_in_ansector);
    }


#if PARENT_PTR == 1
    // find the length of the path ancestor->...->child (excluding the ancestor and including the child)
    static int find_path_len(trie_node *ancestor, trie_node *child) {
        if (child == nullptr || ancestor == nullptr) return 0;

        int depth = 0;
        while (child != ancestor) {
            child = child->parent;
            depth++;
        }

        return depth;
    }
#endif

#if PARENT_PTR == 1
#if VAL_AT_NODE == 1
    // if reversed is false, write the string of the downwards path ancestor->...->child into the dest ptr
    // if reversed is true, write the string of the upwards path child->...->ancestor into the dest ptr
    static std::vector<TYPE> get_path_string(trie_node *ancestor, trie_node *child, bool reversed = false) {
        if (child == nullptr || ancestor == nullptr) return {};

        std::vector<TYPE> str;

        while (child != ancestor) {
            str.push_back(child->val);
            child = child->parent;
        }

        if (!reversed) std::reverse(str.begin(), str.end());
        return str;
    }
#endif
#endif

    static std::vector<trie_node *> get_all_strings(trie_node *node) {
        if (node == nullptr) return {};

        std::vector<trie_node *> res;

        if (node->marked) res.push_back(node);

        for (int i = 0; i < RANGE; i++) {
            trie_node *child = node->children[i];
            if (child == nullptr) continue; // child doesn't exists

            for (trie_node *str: get_all_strings(child))
                res.push_back(str);
        }

        return res;
    }


    // does the trie contain this string
    static bool contains_string(trie_node *node, const TYPE *str, int len) {
        node = search_string(node, str, len);
        return node != nullptr && node->marked;
    }

    // is there a string in the trie which str is a prefix of
    static bool is_prefix(trie_node *node, const TYPE *str, int len) {
        node = search_string(node, str, len);
        return node != nullptr && node->marked || number_of_children(node) > 0;
    }

    // find the k-th (0-indexed) smallest/largest prefix of str which is in the trie
    static trie_node *find_prefix(trie_node *node, const TYPE *str, int len, int k = 0, bool smallest = true) {
        if (node == nullptr) return nullptr;

        if (smallest) {
            int i = 0;
            while (i++ < len) {
                node = search_child(node, str++);
                if (node == nullptr) return nullptr; // the k-th prefix does't exists

                if (node->marked && k-- == 0) return node; // this is the k-th prefix found
            }

            return nullptr;
        } else {
            std::queue<trie_node *> prefs;
            int i = 0;
            while (i++ < len) {
                node = search_string(node, str++);
                if (node == nullptr) break;

                if (node->marked) {
                    if (prefs.size() == k) prefs.pop();
                    prefs.push(node);
                }
            }

            return prefs.size() == k ? prefs.front() : nullptr;
        }
    }

};

#undef PARENT_PTR
#undef VAL_AT_NODE
#undef CUSTOM_DATA_AT_NODE
#undef DEALLOCTE_MEM



#endif //TRIE_H
