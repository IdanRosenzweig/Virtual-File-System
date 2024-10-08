#ifndef SYS_CTL_DEV_H
#define SYS_CTL_DEV_H

#include "src/comp/content/ctl_dev/ctl_dev.h"

struct sys_ctl_dev : public ctl_dev {
    enum codes : code_t {
        GET_SCHED_LOG, GET_ALL_USERS, GET_KERNEL_MODULES
    };

    int access(code_t code, byte* data, int data_sz, byte* dest) override;
    int access(code_t* codes, int code_sz, byte* data, int data_sz, byte* dest) override;
};

#endif //SYS_CTL_DEV_H
