#ifndef AVL_TREE_H
#define AVL_TREE_H

template<typename TYPE, typename DATA>
struct avl_node {
private:
    /** internal data and functions */
    struct avl_node *left = nullptr;
    struct avl_node *right = nullptr;
    struct avl_node *parent = nullptr;

    int height = 1;
    int balance_factor = 0;

    static int get_height(avl_node *node) {
        return node == nullptr ? 0 : node->height;
    }

    static int get_balance_factor(avl_node *node) {
        return node == nullptr ? 0 : node->balance_factor;
    }

    static void update(avl_node *node) {
        if (node == nullptr) return;

        int left_h = get_height(node->left);
        int right_h = get_height(node->right);
        node->height = 1 + (left_h > right_h ? left_h : right_h);;
        node->balance_factor = left_h - right_h;
    }

    static avl_node *left_rotation(avl_node *node) {
        avl_node *y = node->right;

        node->right = y->left;
        y->left = node;

        update(node);
        update(y);

        return y;
    }

    static avl_node *right_rotation(avl_node *node) {
        avl_node *x = node->left;

        node->left = x->right;
        x->right = node;

        update(node);
        update(x);

        return x;
    }

    static avl_node *left_right_rotation(avl_node *node) {
        node->left = left_rotation(node->left);
        update(node);
        node = right_rotation(node);

        return node;
    }

    static avl_node *right_left_rotation(avl_node *node) {
        node->right = right_rotation(node->right);
        update(node);
        node = left_rotation(node);

        return node;
    }

public:
    TYPE value;
    DATA data;

    explicit avl_node(const TYPE &value, const DATA &data) : value(value), data(data) {
    }

    static avl_node* begin(avl_node* root) {
        while (root != nullptr && root->left != nullptr) root = root->left;
        return root;
    }

    static avl_node* end(avl_node* root) {
        return nullptr;
    }

    static avl_node *next(avl_node *node) {
        if (node == nullptr) return nullptr;

        if (node->right != nullptr) {
            node = node->right;
            while (node->left != nullptr) node = node->left;
        } else if (node->parent != nullptr) {
            while (true) {
                if (node->parent == nullptr) {
                    node = nullptr;
                    break;
                } else {
                    if (node->parent->left == node) {
                        node = node->parent;
                        break;
                    } else node = node->parent;
                }
            }
        } else node = nullptr;

        return node;
    }

    static avl_node *prev(avl_node *node) {
        if (node == nullptr) return nullptr;

        if (node->left != nullptr) {
            node = node->left;
            while (node->right != nullptr) node = node->right;
        } else if (node->parent != nullptr) {
            while (true) {
                if (node->parent == nullptr) {
                    node = nullptr;
                    break;
                } else {
                    if (node->parent->right == node) {
                        node = node->parent;
                        break;
                    } else node = node->parent;
                }
            }
        } else node = nullptr;

        return node;
    }

    static avl_node *binary_search(avl_node *node, const TYPE &val, bool lower_bound = true) {
        avl_node *res = nullptr;
        while (node != nullptr) {
            if (node->value < val) node = node->right;
            else if (node->value == val) {
                if (lower_bound) res = node;
                node = node->right;
            } else {
                if (!lower_bound) res = node;
                node = node->left;
            }
        }

        return res;
    }

    static bool count(avl_node *node, const TYPE &value) {
        return binary_search(node, value, true) != nullptr;
    }

    static avl_node *insert(avl_node *node, const TYPE &value, const DATA &data) {
        if (node == nullptr) return new avl_node(value, data);

        // if (value == node->value) return node;

        if (value < node->value) {
            // go left
            if (node->left != nullptr) node->left = insert(node->left, value, data);
            else node->left = new avl_node(value, data);
        } else {
            if (node->right != nullptr) node->right = insert(node->right, value, data);
            else node->right = new avl_node(value, data);
        }

        update(node);

        if (get_balance_factor(node) > 1) {
            if (value < node->left->value) node = right_rotation(node);
            else node = left_right_rotation(node);
        } else if (get_balance_factor(node) < -1) {
            if (!(value < node->right->value)) node = left_rotation(node);
            else node = right_left_rotation(node);
        }

        return node;
    }

    static avl_node *remove(avl_node *node, const TYPE &value) {
        if (node == nullptr) return nullptr;

        if (node->value == value) {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            } else if (node->left == nullptr) {
                avl_node *child = node->right;
                delete node;
                return child;
            } else if (node->right == nullptr) {
                avl_node *child = node->left;
                delete node;
                return child;
            } else {
                avl_node* inorder_successor = next(node);

                node->value = inorder_successor->value;
                node->right = remove(node->right, inorder_successor->value);
            }
        } else if (value < node->value) node->left = remove(node->left, value);
        else node->right = remove(node->right, value);

        update(node);

        if (get_balance_factor(node) > 1) {
            if (!(get_balance_factor(node->left) < 0)) node = right_rotation(node);
            else node = left_right_rotation(node);
        } else if (get_balance_factor(node) < -1) {
            if (get_balance_factor(node->right) < 0) node = left_rotation(node);
            else node = right_left_rotation(node);
        }

        return node;
    }

};


#endif //AVL_TREE_H
