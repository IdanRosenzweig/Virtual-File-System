#ifndef CTL_DEV_H
#define CTL_DEV_H

#include "lib/utility.h"

// control device
struct ctl_dev {
    using code_t = byte;

    virtual void access(code_t code, byte* data, int data_sz, byte* res) = 0;
    virtual void access(code_t* codes, int code_sz, byte* data, int data_sz, byte* res) = 0;

    virtual ~ctl_dev() {}
};

#endif //CTL_DEV_H