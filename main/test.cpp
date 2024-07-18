#include <chrono>
#include <iostream>
#include <string.h>

#include "drivers/fs_ram_driver.h"
#include "fss/tmpfs.h"

fs_ram_driver driver; // sample ram fs driver
tmpfs fs(10, &driver); // sample tmp filesystem

int main() {

    printf("sizeof: %llu\n", sizeof(data));

    node_id bin_node = fs.mk_dir(fs.get_root_node_id(), parse_path("/bin"));

    text_file file1;
    strcpy((char *) file1.text, "this is runme file");
    fs.mk_textfile(fs.get_root_node_id(), parse_path("/bin/runme"), file1);

    // text_file file2;
    // strcpy((char *) file2.text, "this is exec file");
    // fs.mk_textfile(fs.get_root_node_id(), parse_path("/bin/exec"), file2);

    node_id lib_node = fs.mk_dir(fs.get_root_node_id(), parse_path("/lib"));

    node_id usr_node = fs.mk_dir(fs.get_root_node_id(), parse_path("/usr"));
    node_id home_node = fs.mk_dir(fs.get_root_node_id(), parse_path("/home/user"));

    soft_link link;
    link.path_ptr = parse_path("/bin/");
    fs.mk_softlink(home_node, parse_path("bin"), link);

    {
        auto file = fs.open_textfile(fs.search_path(fs.get_root_node_id(), parse_path("/home/user/bin/runme")));
        open_file fd{file.get(), 0};

#define BUFF_SZ 30
        uint8_t buff[BUFF_SZ + 1] = {0};

        read(fd, buff, BUFF_SZ);
        std::cout << buff << std::endl;

        const char *my_data = "new overriding data";
        seek(fd, 0);
        write(fd, (uint8_t *) my_data, strlen(my_data));

        seek(fd, 0);
        read(fd, buff, BUFF_SZ);
        std::cout << buff << std::endl;

        fs.close_textfile(file.get());
    }

    {
        auto file = fs.open_textfile(fs.search_path(fs.get_root_node_id(), parse_path("/home/user/bin/runme")));
        open_file fd{file.get(), 0};

#define BUFF_SZ 30
        uint8_t buff[BUFF_SZ + 1] = {0};

        read(fd, buff, BUFF_SZ);
        std::cout << buff << std::endl;

        const char *my_data = "even new data";
        seek(fd, 0);
        write(fd, (uint8_t *) my_data, strlen(my_data));

        seek(fd, 0);
        read(fd, buff, BUFF_SZ);
        std::cout << buff << std::endl;

        fs.close_textfile(file.get());
    }

    // auto home_path = parse_path("/home/user/files/");
    // fs.mk_dir(fs.get_root_node_id(), home_path);
    // fs.mk_textfile(fs.get_root_node_id(), parse_path("/home/user/files/abcd"), text_file{"first file text"});
    //
    // node_id home_node = fs.search_path(fs.get_root_node_id(), home_path);
    // node_id file_node = fs.search_path_comp(home_node, path_comp{(uint8_t*) "abcd"});

    // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    // for (int i = 0; i < 10000; i++) {
    //     if (home_node != node_id_null) {
    //         auto stat = fs.stat_node(home_node);
    //         // cout << "stated gdb node\n";
    //     }
    //     if (file_node != node_id_null) {
    //         auto stat = fs.stat_node(file_node);
    //         // cout << "stated filetxt node\n";
    //         auto content = fs.stat_node_data(stat.ptr->data_id);
    //         // cout << std::get<0>(std::get<2>(content->data)ata).text << endl;
    //         // auto content = fs.()
    //     }
    // }
    // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::millids>(end - begin).count() <<
    //         "[ms]" << std::endl;
}
