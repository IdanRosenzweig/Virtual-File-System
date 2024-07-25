#ifndef COMP_H
#define COMP_H

#include <variant>
#include "lib/lib_common.h"

#include "comp_id.h"

#include "null_comp/null_comp.h"
#include "dir/dir.h"
#include "softlink/softlink.h"
#include "hardlink/hardlink.h"
#include "mount/mount.h"
#include "textfile/textfile.h"

enum class comp_type {
    null, dir, softlink, hardlink, mount, textfile
};

// all possible types of components
using comp_data = std::variant<null_comp, dir, softlink, hardlink, mount, textfile>; // polymorphic data

struct comp : public comp_data {
    static constexpr inline comp_type get_type(const comp &val) {
        switch (val.index()) {
            default:
            case variant_index<null_comp, comp_data>: return comp_type::null;
            case variant_index<dir, comp_data>: return comp_type::dir;
            case variant_index<softlink, comp_data>: return comp_type::softlink;
            case variant_index<hardlink, comp_data>: return comp_type::hardlink;
            case variant_index<mount, comp_data>: return comp_type::mount;
            case variant_index<textfile, comp_data>: return comp_type::textfile;
        }
    }

    static constexpr inline comp_id_t get_id(const comp &val) {
        return std::visit([&](auto &obj) { return obj.id; }, val);
    }

    template<typename T>
    static constexpr inline T *get_ptr(comp *val) {
        return (T *) std::get_if<T>(val);
    }

    comp() : comp_data(null_comp()) {
    }

    explicit comp(const comp_data &val)
        : comp_data(val) {
    }

    comp(const comp &other)
        : comp_data(other) {
    }

    comp(comp &&other) noexcept
        : comp_data(std::move(other)) {
    }

    comp &operator=(const comp &other) {
        if (this == &other)
            return *this;
        comp_data::operator =(other);
        return *this;
    }

    comp &operator=(comp &&other) noexcept {
        if (this == &other)
            return *this;
        comp_data::operator =(std::move(other));
        return *this;
    }
};

#endif //COMP_H
