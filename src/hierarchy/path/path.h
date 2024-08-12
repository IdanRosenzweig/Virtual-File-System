#ifndef PATH_H
#define PATH_H

#include "path_comp.h"

using path = ::vector<path_comp>;
// using path = std::basic_string<path_comp>;

template <typename CHAR_T>
path parse_path(const CHAR_T* src, int cnt = -1) {
    if (cnt == 0) return {};

    path p;

    int curr_in = 0;
    if (src[curr_in] == '/') {
        // root = true
        curr_in++;
    } // else root = false

    path_comp curr_comp;
    while (curr_in < cnt || (cnt == -1 && src[curr_in] != 0)) {
        if (src[curr_in] == '/') {
            if (!curr_comp.empty()) {
                p.push_back(curr_comp);
                curr_comp.clear();
                curr_in++;
            }
        } else {
            curr_comp.push_back((byte) src[curr_in++]);
        }
    }
    if (!curr_comp.empty())
        p.push_back(curr_comp);

    return ::move(p);
}


#endif //PATH_H
