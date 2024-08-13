#ifndef COMP_H
#define COMP_H

#include "lib/union_variant.h"
#include "lib/lib_common.h"

#include "comp_id.h"

#include "null_comp/null_comp.h"
#include "dir/dir.h"
#include "softlink/softlink.h"
#include "mount/mount.h"
#include "content/content_pt.h"

enum class comp_type {
    null, dir, softlink, mount, content_pt
};

// all possible types of components
using comp_data = union_variant<null_comp, dir, softlink, mount, content_pt>; // polymorphic data

struct comp_t : public comp_data {
    int hardlinks_cnt = 0;

    static inline comp_type get_type(const comp_t &val) {
        switch (val.curr_index()) {
            default:
            case comp_data::type_index<null_comp>(): return comp_type::null;
            case comp_data::type_index<dir>(): return comp_type::dir;
            case comp_data::type_index<softlink>(): return comp_type::softlink;
            case comp_data::type_index<mount>(): return comp_type::mount;
            case comp_data::type_index<content_pt>(): return comp_type::content_pt;
        }
    }

    struct get_id_visitor {
        template<typename T>
        auto operator()(T *ptr) {
            return ptr->id;
        }

        void operator()(void) {
        }
    };

    static inline comp_id_t get_id(const comp_t &val) {
        return ((comp_data &) val).visit(get_id_visitor());
        // return std::visit([&](auto &obj) { return obj.id; }, val);
    }

    struct set_id_visitor {
        comp_id_t id;

        template<typename T>
        auto operator()(T *ptr) {
            ptr->id = id;
        }

        void operator()(void) {
        }
    };

    static inline void set_id(const comp_t &val, comp_id_t id) {
        ((comp_data &) val).visit(set_id_visitor(id));
    }

    template<typename T>
    static inline T *get_ptr(comp_t *val) {
        return val->get_by_type<T>();
        // return (T *) std::get_if<T>(val);
    }

    comp_t() : comp_data(null_comp()) {
    }

    explicit comp_t(const comp_data &val)
        : comp_data(val) {
    }

    comp_t(const comp_t &other)
        : comp_data(other) {
    }

    comp_t(comp_t &&other) noexcept
        : comp_data(move(other)) {
    }

    comp_t &operator=(const comp_t &other) {
        if (this == &other)
            return *this;
        comp_data::operator =(other);
        return *this;
    }

    comp_t &operator=(comp_t &&other) noexcept {
        if (this == &other)
            return *this;
        comp_data::operator =(move(other));
        return *this;
    }
};

#endif //COMP_H
