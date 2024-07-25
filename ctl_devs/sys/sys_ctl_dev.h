#ifndef SYS_CTL_DEV_H
#define SYS_CTL_DEV_H

#include "src/hierarchy/content/ctl_dev/ctl_dev.h"

struct sys_ctl_dev : public ctl_dev {
    enum codes : code_t {
        GET_SCHED_LOG, GET_ALL_USERS, GET_KERNEL_MODULES
    };

    out_data_t access(const code_t &ctl_code, const in_data_t &data) override;
    out_data_t access(const arr_code_t &ctl_arr_code, const in_data_t &data) override;
};

#endif //SYS_CTL_DEV_H
