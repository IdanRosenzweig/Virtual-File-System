#ifndef CONTENT_H
#define CONTENT_H

#include "lib/lib_common.h"
#include "lib/union_variant.h"

#include "content_id.h"

#include "null_content/null_content.h"
// #include "textfile/textfile.h"
#include "ctl_dev/ctl_dev_pt.h"
#include "stream_dev/stream_dev_pt.h"
#include "textfile/textfile.h"

enum class content_type {
    null, textfile
};

// all possible types of content data
using content_data = union_variant<null_content, textfile>; // polymorphic data

struct content_t : public content_data {
    static inline content_type get_type(const content_t &val) {
        switch (val.curr_index()) {
            default:
            case content_data::type_index<null_content>(): return content_type::null;
            case content_data::type_index<textfile>(): return content_type::textfile;
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

    static inline content_id_t get_id(const content_t &val) {
        return ((content_data &) val).visit(get_id_visitor());
        // return std::visit([&](auto &obj) { return obj.id; }, val);
    }

    template<typename T>
    static inline T *get_ptr(content_t *val) {
        return val->get_by_type<T>();
        // return (T *) std::get_if<T>(val);
    }

    content_t() : content_data(null_content()) {
    }

    explicit content_t(const content_data &val)
        : content_data(val) {
    }

    content_t(const content_t &other)
        : content_data(other) {
    }

    content_t(content_t &&other) noexcept
        : content_data(std::move(other)) {
    }

    content_t &operator=(const content_t &other) {
        if (this == &other)
            return *this;
        content_data::operator =(other);
        return *this;
    }

    content_t &operator=(content_t &&other) noexcept {
        if (this == &other)
            return *this;
        content_data::operator =(std::move(other));
        return *this;
    }
};


#endif //CONTENT_H
