#ifndef MOUNT_H
#define MOUNT_H

#include "src/hierarchy/data/base_data.h"
#include "src/fs/fs_id.h"

struct mount : public base_data {
    fs_id mnt_fs;

    mount() : base_data(), mnt_fs(fs_id_null) {
    }
};
#endif //MOUNT_H
