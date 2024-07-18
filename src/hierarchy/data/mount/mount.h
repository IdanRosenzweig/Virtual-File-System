#ifndef MOUNT_H
#define MOUNT_H

#include <utility>

#include "src/hierarchy/data/base_data.h"
#include "src/fs/fs_id.h"

struct mount : public base_data {
    fs_id mnt_fs;

    mount() : base_data(), mnt_fs(fs_id_null) {
    }

    explicit mount(fs_id mnt_fs)
        : mnt_fs(mnt_fs) {
    }

    mount(const mount &other)
        : base_data(other),
          mnt_fs(other.mnt_fs) {
    }

    mount(mount &&other) noexcept
        : base_data(std::move(other)),
          mnt_fs(other.mnt_fs) {
        other.mnt_fs = fs_id_null;
    }

    mount & operator=(const mount &other) {
        if (this == &other)
            return *this;
        base_data::operator =(other);
        mnt_fs = other.mnt_fs;
        return *this;
    }

    mount & operator=(mount &&other) noexcept {
        if (this == &other)
            return *this;
        base_data::operator =(std::move(other));
        mnt_fs = other.mnt_fs;
        other.mnt_fs = fs_id_null;
        return *this;
    }
};
#endif //MOUNT_H
