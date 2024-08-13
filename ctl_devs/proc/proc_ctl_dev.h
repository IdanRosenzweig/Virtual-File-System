#ifndef PROC_CTL_DEV_H
#define PROC_CTL_DEV_H

#include "src/comp/content/ctl_dev/ctl_dev.h"

struct proc_ctl_dev : public ctl_dev {
    enum codes : code_t {
        GET_ARGS, GET_ENVIRON, GET_LIMITS, GET_NS, GET_MOUNT_INFO,
        GET_CWD_LINK, GET_ROOT_LINK
    };

    void access(code_t code, byte* data, int data_sz, byte* res) override;
    void access(code_t* codes, int code_sz, byte* data, int data_sz, byte* res) override;
};



#endif //PROC_CTL_DEV_H
