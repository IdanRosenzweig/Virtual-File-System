#ifndef HARDLINK_H
#define HARDLINK_H

#include <memory>
#include <vector>

#include "../base_comp.h"
#include "src/hierarchy/ctx_t.h"
#include "src/hierarchy/node/node_id.h"

struct comp;

// a node that points to another path
struct hardlink : public base_comp {
    ctx_t<node_id_t> target;

    hardlink() = default;

    explicit hardlink(comp_id_t id)
        : base_comp(id) {
    }

    explicit hardlink(const ctx_t<node_id_t> &target)
        : target(target) {
    }

    hardlink(const hardlink &other)
        : base_comp(other),
          target(other.target) {
    }

    hardlink(hardlink &&other) noexcept
        : base_comp(std::move(other)),
          target(std::move(other.target)) {
    }

    hardlink & operator=(const hardlink &other) {
        if (this == &other)
            return *this;
        base_comp::operator =(other);
        target = other.target;
        return *this;
    }

    hardlink & operator=(hardlink &&other) noexcept {
        if (this == &other)
            return *this;
        base_comp::operator =(std::move(other));
        target = std::move(other.target);
        return *this;
    }

    static void nullify_link(const ctx_t<std::unique_ptr<comp>>& link_comp, bool update_reference);
};


#endif //HARDLINK_H
