#ifndef CTL_DEV_H
#define CTL_DEV_H

#include <cstdint>
#include <vector>
#include <string>

// control device
struct ctl_dev {
    using code_t = uint8_t;
    using arr_code_t = std::vector<uint8_t>;
    using in_data_t = std::basic_string<uint8_t>;
    using out_data_t = std::basic_string<uint8_t>;

    virtual out_data_t access(const code_t& ctl_code, const in_data_t& data) = 0;
    virtual out_data_t access(const arr_code_t& ctl_arr_code, const in_data_t& data) = 0;

    virtual ~ctl_dev() {}
};

#endif //CTL_DEV_H
