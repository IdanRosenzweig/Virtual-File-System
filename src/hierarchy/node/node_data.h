#ifndef NODE_DATA_H
#define NODE_DATA_H

#include <variant>

#include "../null_node/null_node.h"
#include "../object/object.h"
#include "../dir/dir.h"

enum class node_data_type {
    NULL_NODE, DIR, OBJ
};
using node_data = std::variant<null_node, dir, object>;

#endif //NODE_DATA_H
