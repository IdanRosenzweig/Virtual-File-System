#ifndef NODE_ID_H
#define NODE_ID_H

#include "lib/num_wrapper.h"

type_define(node_id_t, unsigned int)

inline node_id_t node_id_null(0);
inline node_id_t node_id_virt(1);

inline node_id_t min_accessable_node_id(2);

#endif //NODE_ID_H
