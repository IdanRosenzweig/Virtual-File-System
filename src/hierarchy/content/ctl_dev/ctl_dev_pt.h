#ifndef CNT_DEV_PT_H
#define CNT_DEV_PT_H

#include "../base_content.h"
#include "ctl_dev.h"

struct ctl_dev_pt : public base_content {
    ctl_dev* dev_ptr = nullptr;

    ctl_dev_pt() = default;

    explicit ctl_dev_pt(ctl_dev *dev_ptr)
        : dev_ptr(dev_ptr) {
    }

    ctl_dev_pt(const ctl_dev_pt &other)
        : base_content(other),
          dev_ptr(other.dev_ptr) {
    }

    ctl_dev_pt(ctl_dev_pt &&other) noexcept
        : base_content(std::move(other)),
          dev_ptr(other.dev_ptr) {
    }

    ctl_dev_pt & operator=(const ctl_dev_pt &other) {
        if (this == &other)
            return *this;
        base_content::operator =(other);
        dev_ptr = other.dev_ptr;
        return *this;
    }

    ctl_dev_pt & operator=(ctl_dev_pt &&other) noexcept {
        if (this == &other)
            return *this;
        base_content::operator =(std::move(other));
        dev_ptr = other.dev_ptr;
        return *this;
    }
};

#endif //CNT_DEV_PT_H
