#include <chrono>
#include <iostream>
#include <string.h>

#include <sys/stat.h>

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
    ctx_t<node_id_t> bin_node = hierarchy::mk_dir(fs->get_root_ctx(), parse_path("/bin"));

    auto open_file1 = hierarchy::open_textfile(hierarchy::mk_textfile(bin_node, parse_path("runme")));
    strcpy((char *) open_file1.val->text, "this is runme file");
    std::cout << "first time: " << open_file1.val->text << std::endl;
    hierarchy::close_textfile(fs, open_file1.val.get());


    // auto open_file2 = hierarchy::open_textfile(hierarchy::mk_textfile(bin_node, parse_path("exec")));
    auto open_file2 = hierarchy::open_textfile(hierarchy::mk_hardlink(fs->get_root_ctx(), parse_path("/bin/runme"), parse_path("/bin/exec")));
    strcpy((char *) open_file2.val->text, "this is exec file");
    std::cout << "second time: " << open_file2.val->text << std::endl;
    hierarchy::close_textfile(fs, open_file2.val.get());


    /* /home/user dir */
    ctx_t<node_id_t> home_id = hierarchy::mk_dir(fs->get_root_ctx(), parse_path("/home/user"));
    hierarchy::mk_softlink(home_id, parse_path("bin"), softlink({&main_fs, parse_path("/bin")})); // from the user home dir

    auto open_tmpfile = hierarchy::open_textfile(hierarchy::mk_textfile(home_id, parse_path("tmpfile")));
    strcpy((char *) open_tmpfile.val->text, "this is a template file");
    hierarchy::close_textfile(fs, open_tmpfile.val.get());


    hierarchy::rm_node(hierarchy::search_node(fs->get_root_ctx(), parse_path("/home/user/bin/runme")), true);
    auto open_file3 = hierarchy::open_textfile(hierarchy::search_node(fs->get_root_ctx(), parse_path("/home/user/bin/exec")));
    std::cout << "third time: " << open_file3.val->text << std::endl;
    strcpy((char *) open_file3.val->text, "this is a file");
    std::cout << "fourth time: " << open_file3.val->text << std::endl;
    hierarchy::close_textfile(fs, open_file3.val.get());


    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for (int i = 0; i < 100000; i++) {
        if (!is_ctx_null(home_id)) {
            auto stat = hierarchy::stat_node(home_id);
        }
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() <<
            "[ms]" << std::endl;
}
