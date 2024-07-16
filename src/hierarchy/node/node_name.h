#ifndef NODE_NAME_H
#define NODE_NAME_H

#include <string>
#include <cstdint>

using node_name = std::basic_string<uint8_t>;
inline node_name node_name_null{};

#endif //NODE_NAME_H
