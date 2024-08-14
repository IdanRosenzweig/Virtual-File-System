cmake_minimum_required(VERSION 3.28)
project(src)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(vfs
        # hierarchy
        src/hierarchy/hierarchy.h
        src/hierarchy/hierarchy.cpp
        src/hierarchy/basic_driver.h
        src/hierarchy/hier_id.h
        src/hierarchy/ctx_t.h

        # node
        src/node/node_id.h

        # path
        src/path/path.h
        src/path/path.cpp
        src/path/path_comp.h

        # comp
        src/comp/comp_t.h
        src/comp/comp_id.h
        src/comp/base_comp.h

        src/comp/null_comp/null_comp.h

        # dir comp
        src/comp/dir/dir.h
        src/comp/dir/dir.cpp

        # softlink comp
        src/comp/softlink/softlink.h

        # refs
        src/refs/refs_t.h
        src/refs/refs_id.h

        # content
        src/comp/content/content_t.h
        src/comp/content/content_id.h
        src/comp/content/base_content.h
        src/comp/content/content_pt.h

        src/comp/content/null_content/null_content.h

        src/comp/content/textfile/textfile.cpp
        src/comp/content/textfile/textfile.h

        src/comp/content/ctl_dev/ctl_dev_pt.h
        src/comp/content/ctl_dev/ctl_dev.h

        src/comp/content/stream_dev/stream_dev.h
        src/comp/content/stream_dev/stream_dev_pt.h

        src/comp/content/block_dev/block_dev.h

        # mount
        src/comp/mount/mount.h
        src/comp/mount/mount.h

        # drivers
        drivers/ram_driver.cpp
        drivers/ram_driver.h
        drivers/ext4_fs.cpp
        drivers/ext4_fs.h

        # ctl devs
        ctl_devs/proc/proc_ctl_dev.cpp
        ctl_devs/proc/proc_ctl_dev.h

        ctl_devs/sys/sys_ctl_dev.cpp
        ctl_devs/sys/sys_ctl_dev.h

        ctl_devs/hw/hw_ctl_dev.cpp
        ctl_devs/hw/hw_ctl_dev.h

        # stream devs
        stream_devs/random_stream_dev.cpp
        stream_devs/random_stream_dev.h

        # lib
        lib/id_manager.h
        lib/lib_common.h
        lib/union_variant.h
        lib/avl_tree.h
        lib/queue.h
        lib/utility.h
        lib/utility.cpp
        lib/vector.h
        lib/lib_common.cpp
        lib/num_wrapper.h

        # app
        app/test.cpp
#        app/module.cpp

)
