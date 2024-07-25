#ifndef ID_MANAGER_H
#define ID_MANAGER_H

#include <queue>

#include "num_wrapper.h"

template<typename UNUM_T>
struct id_manager {
private:
    UNUM_T next;
    std::queue<UNUM_T> freed;

public:
    id_manager(UNUM_T min_val) : next(min_val) {
    }

    UNUM_T generate_id() {
        UNUM_T id;
        if (!freed.empty()) {
            id = freed.front();
            freed.pop();
        } else id = next++;

        return id;
    }

    void free_id(UNUM_T id) {
        freed.push(id);
    }
};

#endif //ID_MANAGER_H
