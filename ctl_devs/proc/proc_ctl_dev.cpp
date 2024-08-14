#include "proc_ctl_dev.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/limits.h>
#include "errno.h"

int proc_ctl_dev::access(code_t code, byte *data, int data_sz, byte *dest) {
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
            if (fd == -1) return 0;

            cnt = read(fd, args, ARGS_MAX);
            close(fd);
            if (cnt == -1) return 0;

            for (int i = 0; i < cnt; i++) {
                if (args[i] == 0 && i + 1 != cnt) dest[i] = ' ';
                else dest[i] = args[i];
            }

            return cnt;
        }
        case codes::GET_ENVIRON: {
            char path[PATH_MAX] = {};
            snprintf(path, PATH_MAX, "/proc/%d/environ", pid);

#define ENVIRON_MAX 2000
            char environ[ENVIRON_MAX] = {};
            int cnt;

            int fd = open(path, O_RDONLY);
            if (fd == -1) return 0;

            cnt = read(fd, environ, ENVIRON_MAX);
            close(fd);
            if (cnt == -1) return 0;
            for (int i = 0; i < cnt; i++) {
                if (environ[i] == 0 && i + 1 != cnt) dest[i] = ' ';
                else dest[i] = environ[i];
            }

            return cnt;
        }
        case codes::GET_LIMITS: {
            break;
        }
        case codes::GET_NS: {
            char dir[PATH_MAX] = {};
            snprintf(dir, PATH_MAX, "/proc/%d/ns", pid);

#define LIMITS_MAX 200
            char spaces[LIMITS_MAX] = {};

            static char *ns[] = {"/user", "/pid", "/mnt", "/net", "/ipc", "/cgroup", "/time", "/uts"};

            char path[PATH_MAX] = {};
            int cnt = 0;
            for (int i = 0; i < sizeof(ns); i++) {
                snprintf(path, PATH_MAX, "%s/%s", dir, ns[i]);

                int ret = readlink(path, spaces + cnt, LIMITS_MAX - cnt);
                if (ret == -1) return 0;

                cnt += ret;
                if (i + 1 != sizeof(ns)) spaces[cnt - 1] = ' ';
            }

            memcpy(dest, spaces, cnt);
            return cnt;
        }
        case codes::GET_MOUNT_INFO: {
            break;
        }
        case codes::GET_CWD_LINK: {
            char path[PATH_MAX] = {};
            snprintf(path, PATH_MAX, "/proc/%d/cwd", pid);

            char cwd[PATH_MAX] = {};

            int ret = readlink(path, cwd, PATH_MAX);
            if (ret == -1) return 0;

            memcpy(dest, cwd, ret);
            return ret;
        }
        case codes::GET_ROOT_LINK: {
            char path[PATH_MAX] = {};
            snprintf(path, PATH_MAX, "/proc/%d/root", pid);

            char root[PATH_MAX] = {};

            int ret = readlink(path, root, PATH_MAX);
            if (ret == -1) return 0;

            memcpy(dest, root, ret);
            return ret;
        }
    }
}

int proc_ctl_dev::access(code_t *codes, int code_sz, byte *data, int data_sz, byte *dest) {
    return 0;
}
