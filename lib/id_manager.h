#ifndef ID_MANAGER_H
#define ID_MANAGER_H

#include "queue.h"

#include "num_wrapper.h"

template<typename UNUM_T>
struct id_manager {
private:
    UNUM_T next;
    queue<UNUM_T> freed;

public:
    id_manager(UNUM_T min_val) : next(min_val) {
    }

    UNUM_T generate_id() {
        UNUM_T id;
        if (!freed.empty()) {
            id = freed.front();
            freed.pop_front();
        } else id = next++;

        return id;
    }

    void free_id(UNUM_T id) {
        freed.push_back(id);
    }
};

#endif //ID_MANAGER_H
