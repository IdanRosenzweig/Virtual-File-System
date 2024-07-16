#include <chrono>
#include <iostream>
using namespace std;

#include "samples/ramfs.h"

int main() {
    ramfs fs;

    loaded_node root_node(fs.read_node(fs.get_root_node_id(), true, true));
    cout << "root node id: " << root_node.ptr->id << endl;

    // fs.mk_dir(fs.get_root_node_id(), parse_path("/home/user/ctf/").second);
    // fs.mk_dir(fs.get_root_node_id(), parse_path("/boot").second);
    // fs.mk_dir(fs.get_root_node_id(), parse_path("/lib/x/").second);
    // fs.mk_dir(fs.get_root_node_id(), parse_path("/lib/arm/").second);
    auto parsed = parse_path("/usr/share/gdb");
    node_id dir_id = fs.mk_dir(fs.get_root_node_id(), parsed.second);

    fs.mk_obj(dir_id, path_comp{(uint8_t*) "filetxta"}, obj_data{text_file{"this is some data in a text file"}});
    fs.mk_obj(dir_id, path_comp{(uint8_t*) "filetxtb"}, obj_data{text_file{"this is some data in a text file"}});
    fs.mk_obj(dir_id, path_comp{(uint8_t*) "filetxtc"}, obj_data{text_file{"this is some data in a text file"}});
    fs.mk_obj(dir_id, path_comp{(uint8_t*) "filetxtd"}, obj_data{text_file{"this is some data in a text file"}});
    fs.mk_obj(dir_id, path_comp{(uint8_t*) "filetxte"}, obj_data{text_file{"this is some data in a text file"}});



    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for (int i = 0; i < 10000; i++) {
        node_id gdb_node = fs.search_path(fs.get_root_node_id(), parsed.second);
        if (gdb_node != node_id_null) {
            auto stat = fs.stat_node(gdb_node);
            // cout << "stated gdb node\n";
            // cout << "name: " << stat->name.c_str() << endl;
            // cout << "type: " << (int) node::data_type(*stat) << endl;
        }
        node_id file_node = fs.search_comp(gdb_node, path_comp{(uint8_t*) "filetxta"});
        if (gdb_node != node_id_null) {
            auto stat = fs.stat_node(gdb_node);
            // cout << std::get<0>(std::get<2>(stat.ptr->data).data).text << "\n";
            // cout << "name: " << stat->name.c_str() << endl;
            // cout << "type: " << (int) node::data_type(*stat) << endl;
        }
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() <<
            "[ms]" << std::endl;
}
