#ifndef TMPFS_H
#define TMPFS_H

#include "src/fs/fs.h"

struct tmpfs : public fs {
private:
    // allocate locally
    node_id root_id;

public:
    tmpfs(fs_id id, fs_driver_ops *driver);

    node_id get_root_node_id() override;
};



#endif //TMPFS_H
