#ifndef HW_CTL_DEV_H
#define HW_CTL_DEV_H

#include "src/hierarchy/content/ctl_dev/ctl_dev.h"

struct hw_ctl_dev : public ctl_dev {
    enum codes : code_t {
        GET_DISKS, GET_CPU_INFO, GET_NET_ADAPTERS, GET_BUS_INFO, GET_POWER_INFO
    };

    out_data_t access(const code_t &ctl_code, const in_data_t &data) override;
    out_data_t access(const arr_code_t &ctl_arr_code, const in_data_t &data) override;
};

#endif //HW_CTL_DEV_H
