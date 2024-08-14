#include "proc_ctl_dev.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/limits.h>
#include "errno.h"

void proc_ctl_dev::access(code_t code, byte *data, int data_sz, byte *dest) {
    int pid = 0;
    for (int i = 0, pow = 1; i < data_sz; i++, pow *= 256) pid += data[i] * pow;

    switch (code) {
        case codes::GET_ARGS: {
            char path[PATH_MAX] = {};
            snprintf(path, PATH_MAX, "/proc/%d/cmdline", pid);

#define ARGS_MAX 1000
            char args[ARGS_MAX] = {};
            int cnt;

            int fd = open(path, O_RDONLY);
            if (fd == -1) return;

            cnt = read(fd, args, ARGS_MAX);
            if (cnt == -1) {
                close(fd);
                return;
            }
            for (int i = 0; i < cnt; i++) {
                if (args[i] == 0 && i + 1 != cnt) dest[i] = ' ';
                else dest[i] = args[i];
            }

        stop:
            if (fd != -1) close(fd);
            break;
        }
        case codes::GET_ENVIRON: {
            char path[PATH_MAX] = {};
            snprintf(path, PATH_MAX, "/proc/%d/environ", pid);

#define ENVIRON_MAX 2000
            char environ[ENVIRON_MAX] = {};
            int cnt;

            int fd = open(path, O_RDONLY);
            if (fd == -1) return;

            cnt = read(fd, environ, ENVIRON_MAX);
            if (cnt == -1) {
                close(fd);
                return;
            }
            for (int i = 0; i < cnt; i++) {
                if (environ[i] == 0 && i + 1 != cnt) dest[i] = ' ';
                else dest[i] = environ[i];
            }

            if (fd != -1) close(fd);
            break;
        }
        case codes::GET_LIMITS: {
            break;
        }
        case codes::GET_NS: {
            char dir[PATH_MAX] = {};
            snprintf(dir, PATH_MAX, "/proc/%d/ns", pid);

#define LIMITS_MAX 200
            char limits[LIMITS_MAX] = {};

            static char *ns[] = {"/user", "/pid", "/mnt", "/net", "/ipc", "/cgroup", "/time", "/uts"};

            char path[PATH_MAX] = {};
            int cnt = 0;
            for (int i = 0; i < sizeof(ns); i++) {
                snprintf(path, PATH_MAX, "%s/%s", dir, ns[i]);

                int ret = readlink(path, limits + cnt, LIMITS_MAX - cnt);
                if (ret == -1) return;

                cnt += ret;
                if (i + 1 != sizeof(ns)) limits[cnt - 1] = ' ';
            }

            memcpy(dest, path, cnt);

            break;
        }
        case codes::GET_MOUNT_INFO: {
            break;
        }
        case codes::GET_CWD_LINK: {
            char path[PATH_MAX] = {};
            snprintf(path, PATH_MAX, "/proc/%d/cwd", pid);

            char cwd[PATH_MAX] = {};

            int ret = readlink(path, cwd, PATH_MAX);
            if (ret == -1) return;

            memcpy(dest, path, ret);

            break;
        }
        case codes::GET_ROOT_LINK: {
            char path[PATH_MAX] = {};
            snprintf(path, PATH_MAX, "/proc/%d/root", pid);

            char cwd[PATH_MAX] = {};

            int ret = readlink(path, cwd, PATH_MAX);
            if (ret == -1) return;

            memcpy(dest, path, ret);

            break;
        }
    }
}

void proc_ctl_dev::access(code_t *codes, int code_sz, byte *data, int data_sz, byte *dest) {
}
