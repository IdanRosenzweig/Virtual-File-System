#ifndef MOUNT_H
#define MOUNT_H

#include <utility>

#include "../base_content.h"

struct content_fs;

struct mount : public base_content {
    content_fs* mnt_fs = nullptr;
    int no_sub_mount = 0;

    mount() : base_content() {
    }

    explicit mount(content_fs *mnt_fs)
        : base_content(), mnt_fs(mnt_fs) {
    }

    mount(const mount &other)
        : base_content(other),
          mnt_fs(other.mnt_fs),
          no_sub_mount(other.no_sub_mount) {
    }

    mount(mount &&other) noexcept
        : base_content(std::move(other)),
          mnt_fs(other.mnt_fs),
          no_sub_mount(other.no_sub_mount) {
        other.mnt_fs = nullptr;
        other.no_sub_mount = 0;
    }

    mount & operator=(const mount &other) {
        if (this == &other)
            return *this;
        base_content::operator =(other);
        mnt_fs = other.mnt_fs;
        no_sub_mount = other.no_sub_mount;
        return *this;
    }

    mount & operator=(mount &&other) noexcept {
        if (this == &other)
            return *this;
        base_content::operator =(std::move(other));
        mnt_fs = other.mnt_fs;
        other.mnt_fs = nullptr;
        no_sub_mount = other.no_sub_mount;
        other.no_sub_mount = 0;
        return *this;
    }
};
#endif //MOUNT_H
