#include <chrono>
#include <iostream>

#include "src/hierarchy/hierarchy.h"
#include "drivers/ram_driver.h"
#include "ctl_devs/proc/proc_ctl_dev.h"
#include "stream_devs/random_stream_dev.h"

// a single filesystem based on RAM
ram_driver fs_driver;
hierarchy main_fs(0, &fs_driver);

// process control device
proc_ctl_dev proc_ctl;

// random bytes stream
random_stream_dev random_stream;

int main() {
    auto fs = &main_fs;

    /* /bin dir */
    char *execs[] = {
        "/bin/file",
        "/bin/id",
        "/bin/whoami",
        "/bin/data",
        "/bin/ls",
        "/bin/ps",
        "/bin/htop",
        "/bin/cat",
        "/bin/less",
        "/bin/pwd",
        "/bin/stat",
    };
    for (char *exec: execs)
        hierarchy::mk_textfile(fs->get_root_ctx(), parse_path(exec));


    /* /proc dir */
    char *procs[] = {
        "/proc/auxv",
        "/proc/cmdline",
        "/proc/comm",
        "/proc/cgroup",
        "/proc/environ",
        "/proc/exe",
        "/proc/fd",
        "/proc/limits",
        "/proc/net",
        "/proc/maps",
        "/proc/root",
        "/proc/pagemap",
        "/proc/stack",
        "/proc/syscal",
    };
    for (char *proc: procs)
        hierarchy::mk_textfile(fs->get_root_ctx(), parse_path(proc));


    /* /tmp dir */
    char *tmps[] = {
        "/tmp/file1",
        "/tmp/file2",
        "/tmp/file3",
    };
    for (char *tmp: tmps)
        hierarchy::mk_textfile(fs->get_root_ctx(), parse_path(tmp));


    /* /home dir */
    hierarchy::mk_softlink(fs->get_root_ctx(),
                           parse_path("/home/user/bin"),
                           softlink({&main_fs, parse_path("/bin")})
    );
    hierarchy::mk_softlink(fs->get_root_ctx(),
                           parse_path("/home/user/tmp"),
                           softlink({&main_fs, parse_path("/tmp")})
    );
    hierarchy::mk_softlink(fs->get_root_ctx(),
                           parse_path("/home/user/proc"),
                           softlink({&main_fs, parse_path("/proc")})
    );


    /* textfile test */
    ctx_t<textfile> open_file1;
    hierarchy::open_textfile(hierarchy::mk_textfile(fs->get_root_ctx(), parse_path("/tmp/testfile")), &open_file1);
    strcpy(open_file1.val.text, "this is testfile");
    std::cout << "first time: " << open_file1.val.text << std::endl;
    hierarchy::close_textfile(fs, &open_file1.val);

    ctx_t<textfile> open_file2;
    hierarchy::open_textfile(
        hierarchy::mk_hardlink(fs->get_root_ctx(), parse_path("/tmp/testfile"), parse_path("/links/testfile")),
        &open_file2);
    strcpy(open_file2.val.text, "this is second test");
    std::cout << "second time: " << open_file2.val.text << std::endl;
    hierarchy::close_textfile(fs, &open_file2.val);

    // hierarchy::rm_node(hierarchy::search_node(fs->get_root_ctx(), parse_path("/tmp/testfile")), true);
    ctx_t<textfile> open_file3;
    hierarchy::open_textfile(hierarchy::search_node(fs->get_root_ctx(), parse_path("/links/testfile")),
                             &open_file3);
    std::cout << "third time: " << open_file3.val.text << std::endl;
    strcpy(open_file3.val.text, "this is a file");
    std::cout << "fourth time: " << open_file3.val.text << std::endl;
    hierarchy::close_textfile(fs, &open_file3.val);


    // latency test
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for (int i = 0; i < 100000; i++) {
        ctx_t<node_id_t> home_id = hierarchy::search_node(main_fs.get_root_ctx(), parse_path("/home/user"));
        if (!is_ctx_null(home_id)) {
            ctx_t<node_t> node;
            hierarchy::stat_node(home_id, &node);

            // ctx_t<dir> dir;
            // hierarchy::stat_dir(home_id, &dir);
        }
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() <<
            "[ms]" << std::endl;


    // hierarchy::mk_mount(fs->get_root_ctx(), parse_path("/mnt/proc"), mount())
}
