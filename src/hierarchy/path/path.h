#ifndef PATH_H
#define PATH_H

#include <vector>
#include <string>
#include "path_comp.h"

using path = std::vector<path_comp>;
// using path = std::basic_string<path_comp>;

path parse_path(const std::string& str);


#endif //PATH_H
