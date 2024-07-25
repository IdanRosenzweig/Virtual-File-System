#ifndef STREAM_DEV_PT_H
#define STREAM_DEV_PT_H

#include <utility>

#include "../base_content.h"
#include "stream_dev.h"

struct stream_dev_pt : public base_content {
    stream_dev* dev_ptr = nullptr;

    stream_dev_pt() = default;

    explicit stream_dev_pt(stream_dev *dev_ptr)
        : dev_ptr(dev_ptr) {
    }

    stream_dev_pt(const stream_dev_pt &other)
        : base_content(other),
          dev_ptr(other.dev_ptr) {
    }

    stream_dev_pt(stream_dev_pt &&other) noexcept
        : base_content(std::move(other)),
          dev_ptr(other.dev_ptr) {
    }

    stream_dev_pt & operator=(const stream_dev_pt &other) {
        if (this == &other)
            return *this;
        base_content::operator =(other);
        dev_ptr = other.dev_ptr;
        return *this;
    }

    stream_dev_pt & operator=(stream_dev_pt &&other) noexcept {
        if (this == &other)
            return *this;
        base_content::operator =(std::move(other));
        dev_ptr = other.dev_ptr;
        return *this;
    }
};

#endif //STREAM_DEV_PT_H
