#include "path.h"

path parse_path(const std::string &str) {
    int n = str.size();

    path path;

    int curr_in = 0;
    if (str[curr_in] == '/') {
        // root = true
        curr_in++;
    } // else root = false

    path_comp curr_comp;
    while (curr_in < n) {
        if (str[curr_in] == '/') {
            if (!curr_comp.empty()) {
                path.push_back(curr_comp);
                curr_comp.clear();
                curr_in++;
            }
        } else {
            curr_comp.push_back((uint8_t) str[curr_in++]);
        }
    }
    if (!curr_comp.empty()) path.push_back(curr_comp);

    return path;
}
