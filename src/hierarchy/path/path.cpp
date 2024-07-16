#include "path.h"

std::pair<bool, path> parse_path(const std::string &str) {
    int n = str.size();

    std::pair<bool, path> res;

    int curr_in = 0;
    if (str[curr_in] == '/') {
        res.first = true;
        curr_in++;
    } else res.first = false;

    path_comp curr_comp;
    while (curr_in < n) {
        if (str[curr_in] == '/') {
            if (!curr_comp.empty()) {
                res.second.push_back(curr_comp);
                curr_comp.clear();
                curr_in++;
            }
        } else {
            curr_comp.push_back((uint8_t) str[curr_in++]);
        }
    }
    if (!curr_comp.empty()) res.second.push_back(curr_comp);

    return res;
}
