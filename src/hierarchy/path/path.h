#ifndef PATH_H
#define PATH_H

#include <vector>
#include <string>

#include "../node/node_name.h"

using path_comp = node_name;
using path = std::vector<path_comp>;

std::pair<bool, path> parse_path(const std::string& str);


#endif //PATH_H
