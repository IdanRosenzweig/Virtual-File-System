#ifndef PROC_CTL_DEV_H
#define PROC_CTL_DEV_H

#include "src/hierarchy/content/ctl_dev/ctl_dev.h"

struct proc_ctl_dev : public ctl_dev {
    enum codes : code_t {
        GET_ARGS, GET_ENVIRON, GET_LIMITS, GET_NS, GET_MOUNT_INFO,
        GET_CWD_LINK, GET_ROOT_LINK
    };

    out_data_t access(const code_t &ctl_code, const in_data_t &data) override;
    out_data_t access(const arr_code_t &ctl_arr_code, const in_data_t &data) override;
};



#endif //PROC_CTL_DEV_H
