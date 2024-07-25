#include <chrono>
#include <iostream>
#include <string.h>

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

//     /* /proc */
//     ctx_t<node_id_t> proc_dir = hierarchy::mk_dir(fs->get_root_ctx(), parse_path("/proc"));
//     ctx_t<node_id_t> proc_ctl_node = hierarchy::mk_ctl_dev_pt(proc_dir, parse_path("ctl"), ctl_dev_pt(&proc_ctl));
//
//     // test
//     {
//         auto ctl_ptr = hierarchy::stat_ctl_dev_pt(hierarchy::search_node(main_fs.get_root_ctx(), parse_path("/proc/ctl")));
//         std::cout << ctl_ptr.val->dev_ptr->access(proc_ctl_dev::GET_ARGS, {'1'}).c_str() << std::endl;
//     }
//
//     /* /dev */
//     ctx_t<node_id_t> dev_dir = hierarchy::mk_dir(fs->get_root_ctx(), parse_path("/dev"));
//     hierarchy::mk_stream_dev_pt(dev_dir, parse_path("random"), stream_dev_pt(&random_stream));
//
//     // test
//     {
//         auto random_ptr = hierarchy::stat_stream_dev_pt(hierarchy::search_node(main_fs.get_root_ctx(), parse_path("/dev/random")));
// #define READ_CNT 50
//         char buff[READ_CNT + 1];
//         buff[READ_CNT] = 0;
//         random_ptr.val->dev_ptr->read(buff, READ_CNT);
//         std::cout << buff << std::endl;
//     }


    /* /bin dir */
    ctx_t<node_id_t> bin_node = hierarchy::mk_dir(fs->get_root_ctx(), parse_path("/bin"));

    ctx_t<node_id_t> file1_id = hierarchy::mk_textfile(bin_node, parse_path("runme"));
    auto open_file1 = hierarchy::open_textfile(file1_id);
    strcpy((char *) open_file1.val->text, "this is runme file");
    hierarchy::close_textfile(fs, open_file1.val.get());

    ctx_t<node_id_t> file2_id = hierarchy::mk_textfile(bin_node, parse_path("exec"));
    auto open_file2 = hierarchy::open_textfile(file2_id);
    strcpy((char *) open_file2.val->text, "this is exec file");
    hierarchy::close_textfile(fs, open_file2.val.get());


    /* /home/user dir */
    ctx_t<node_id_t> home_id = hierarchy::mk_dir(fs->get_root_ctx(), parse_path("/home/user"));

    softlink link({&main_fs, parse_path("/bin")}); /* softlink to /bin */
    hierarchy::mk_softlink(home_id, parse_path("bin"), link); // from the user home dir


    ctx_t<node_id_t> tmpfile_id = hierarchy::mk_textfile(home_id, parse_path("tmpfile"));
    auto open_tmpfile = hierarchy::open_textfile(tmpfile_id);
    strcpy((char *) open_tmpfile.val->text, "this is a template file");
    hierarchy::close_textfile(fs, open_tmpfile.val.get());

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for (int i = 0; i < 100000; i++) {
        if (!is_ctx_null(home_id)) {
            auto stat = hierarchy::stat_node(home_id);
        }
        if (!is_ctx_null(tmpfile_id)) {
            auto stat = hierarchy::stat_node(tmpfile_id);
        }
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() <<
            "[ms]" << std::endl;
}
