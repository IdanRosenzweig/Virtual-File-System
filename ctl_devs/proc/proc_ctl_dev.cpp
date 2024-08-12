#include "proc_ctl_dev.h"

#include <fcntl.h>
#include <unistd.h>
#include <linux/limits.h>
#include "errno.h"

void proc_ctl_dev::access(code_t code, byte* data, int data_sz, byte* res) {
//     errno = 0;
//     int pid = strtol((char *) data, nullptr, 10);
//     if (errno != 0) return;
//
//     switch (code) {
//         case codes::GET_ARGS: {
//             char path[PATH_MAX];
//             snprintf(path, 20, "/proc/%d/cmdline", pid);
//
//             int fd = open(path, O_RDONLY);
//             if (fd == -1) throw;
//
// #define ARGS_MAX 1000
//             char args[ARGS_MAX];
//             for (int i = 0; i < ARGS_MAX; i++) args[i] = 0;
//
//             int cnt = read(fd, args, 200);
//             if (cnt <= 0) throw;
//             for (int i = 0; i < cnt; i++)
//                 if (args[i] == 0 && i + 1 != cnt) args[i] = ' ';
//
//             memcpy(res, args, cnt);
//             break;
//         }
//         case codes::GET_ENVIRON: {
//             break;
//         }
//         case codes::GET_LIMITS: {
//             break;
//         }
//         case codes::GET_NS: {
//             break;
//         }
//         case codes::GET_MOUNT_INFO: {
//             break;
//         }
//         case codes::GET_CWD_LINK: {
//             break;
//         }
//         case codes::GET_ROOT_LINK: {
//             break;
//         }
//     }
}

void proc_ctl_dev::access(code_t* codes, int code_sz, byte* data, int data_sz, byte* res) {
}
