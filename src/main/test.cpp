#include <chrono>
#include <iostream>
#include <string.h>

#include "src/drivers/ram_driver.h"
#include "src/content_fs/content_fs.h"
#include "src/mounter/mounter.h"


// the OS's global mounter
ram_driver mounter_driver;
mounter root_mounter(&mounter_driver);

// a main content filesystem based on RAM
ram_driver fs_driver;
content_fs main_fs(99, &fs_driver);

void setup() {
    // mount the main RAM fs in the top dir inside the mounter
    root_mounter.mk_mount(root_mounter.get_root_node_id(), parse_path("/"), mount(&main_fs));
}

int main() {
    setup();
    auto fs = root_mounter.search_underlying_mount(root_mounter.get_root_node_id(), parse_path("/")).first;

    /* /bin dir */
    node_id_t bin_node = fs->mk_dir(fs->get_root_node_id(), parse_path("/bin"));

    node_id_t file1_id = fs->mk_textfile(fs->get_root_node_id(), parse_path("/bin/runme"));
    auto open_file1 = fs->open_textfile(file1_id);
    strcpy((char *) open_file1->text, "this is runme file");
    fs->close_textfile(open_file1.get());

    node_id_t file2_id = fs->mk_textfile(fs->get_root_node_id(), parse_path("/bin/exec"));
    auto open_file2 = fs->open_textfile(file2_id);
    strcpy((char *) open_file2->text, "this is exec file");
    fs->close_textfile(open_file2.get());

    /* /lib dir */
    node_id_t lib_node = fs->mk_dir(fs->get_root_node_id(), parse_path("/lib"));

    /* /usr dir */
    node_id_t usr_node = fs->mk_dir(fs->get_root_node_id(), parse_path("/usr"));

    /* /home/user dir */
    node_id_t home_id = fs->mk_dir(fs->get_root_node_id(), parse_path("/home/user"));

    softlink link(parse_path("/bin")); /* softlink from home dir to bin */
    fs->mk_softlink(home_id, parse_path("bin"), link);


    node_id_t tmpfile_id = fs->mk_textfile(home_id, parse_path("tmpfile"));
    auto open_tmpfile = fs->open_textfile(tmpfile_id);
    strcpy((char *) open_tmpfile->text, "this is a template file");
    fs->close_textfile(open_tmpfile.get());

//     {
//         auto file = fs->open_textfile(fs->search_path(fs->get_root_node_id(), parse_path("/home/user/bin/runme")));
//         open_file fd{file.get(), 0};
//
// #define BUFF_SZ 30
//         uint8_t buff[BUFF_SZ + 1] = {0};
//
//         read(fd, buff, BUFF_SZ);
//         std::cout << buff << std::endl;
//
//         const char *my_data = "new overriding data";
//         seek(fd, 0);
//         write(fd, (uint8_t *) my_data, strlen(my_data));
//
//         seek(fd, 0);
//         read(fd, buff, BUFF_SZ);
//         std::cout << buff << std::endl;
//
//         fs->close_textfile(file.get());
//     }
//
//     {
//         auto file = fs->open_textfile(fs->search_path(fs->get_root_node_id(), parse_path("/home/user/bin/runme")));
//         open_file fd{file.get(), 0};
//
// #define BUFF_SZ 30
//         uint8_t buff[BUFF_SZ + 1] = {0};
//
//         read(fd, buff, BUFF_SZ);
//         std::cout << buff << std::endl;
//
//         const char *my_data = "even new data";
//         seek(fd, 0);
//         write(fd, (uint8_t *) my_data, strlen(my_data));
//
//         seek(fd, 0);
//         read(fd, buff, BUFF_SZ);
//         std::cout << buff << std::endl;
//
//         fs->close_textfile(file.get());
//     }

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for (int i = 0; i < 100000; i++) {
        if (home_id != node_id_null) {
            auto stat = fs->stat_node(home_id);
        }
        if (tmpfile_id != node_id_null) {
            auto stat = fs->stat_node(tmpfile_id);
        }
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() <<
            "[ms]" << std::endl;
}
