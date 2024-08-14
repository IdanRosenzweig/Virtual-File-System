#ifndef HW_CTL_DEV_H
#define HW_CTL_DEV_H

#include "src/comp/content/ctl_dev/ctl_dev.h"

struct hw_ctl_dev : public ctl_dev {
    enum codes : code_t {
        GET_DISKS, GET_CPU_INFO, GET_NET_ADAPTERS, GET_BUS_INFO, GET_POWER_INFO
    };

    int access(code_t code, byte* data, int data_sz, byte* dest) override;
    int access(code_t* codes, int code_sz, byte* data, int data_sz, byte* dest) override;
};

#endif //HW_CTL_DEV_H
