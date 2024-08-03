#include "proc_ctl_dev.h"

#include <fcntl.h>
#include <unistd.h>
#include <linux/limits.h>

ctl_dev::out_data_t proc_ctl_dev::access(const code_t &ctl_code, const in_data_t &data) {
    errno = 0;
    int pid = strtol((char *) data.c_str(), nullptr, 10);
    if (errno != 0) return {};

    switch (ctl_code) {
        case codes::GET_ARGS: {
            char path[PATH_MAX];
            snprintf(path, 20, "/proc/%d/cmdline", pid);

            int fd = open(path, O_RDONLY);
            if (fd == -1) throw;

#define ARGS_MAX 1000
            char args[ARGS_MAX];
            for (int i = 0; i < ARGS_MAX; i++) args[i] = 0;

            int res = read(fd, args, 200);
            if (res <= 0) throw;
            for (int i = 0; i < res; i++)
                if (args[i] == 0 && i + 1 != res) args[i] = ' ';

            return out_data_t((uint8_t*) args, res);
        }
        case codes::GET_ENVIRON: {
            break;
        }
        case codes::GET_LIMITS: {
            break;
        }
        case codes::GET_NS: {
            break;
        }
        case codes::GET_MOUNT_INFO: {
            break;
        }
        case codes::GET_CWD_LINK: {
            break;
        }
        case codes::GET_ROOT_LINK: {
            break;
        }
    }
    return {};
}

ctl_dev::out_data_t proc_ctl_dev::access(const arr_code_t &ctl_arr_code, const in_data_t &data) {
    return {};
}
