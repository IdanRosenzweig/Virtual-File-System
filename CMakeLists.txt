cmake_minimum_required(VERSION 3.28)
project(src)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(vfs
        # hierarchy
        src/hierarchy/hierarchy.h
        src/hierarchy/hierarchy.cpp
        src/hierarchy/basic_driver.h

        src/hierarchy/ctx_t.h

        # node
        src/hierarchy/node/node_id.h

        # path
        src/hierarchy/path/path.h
        src/hierarchy/path/path.cpp
        src/hierarchy/path/path_comp.h

        # comps
        src/hierarchy/comp/comp_t.h
        src/hierarchy/comp/comp_id.h
        src/hierarchy/comp/base_comp.h

        src/hierarchy/comp/null_comp/null_comp.h

        src/hierarchy/comp/dir/dir.h
        src/hierarchy/comp/dir/dir.cpp

        src/hierarchy/comp/softlink/softlink.h

        # refs
        src/hierarchy/refs/refs_t.h
        src/hierarchy/refs/refs_id.h

        # content
        src/hierarchy/content/content_t.h
        src/hierarchy/content/content_id.h
        src/hierarchy/content/base_content.h

        src/hierarchy/content/null_content/null_content.h

        src/hierarchy/content/textfile/textfile.cpp
        src/hierarchy/content/textfile/textfile.h

        src/hierarchy/comp/mount/mount.h
        src/hierarchy/comp/mount/mount.h

        # content fs
        src/hierarchy/hier_id.h

        # drivers
        drivers/ram_driver.cpp
        drivers/ram_driver.h

        drivers/ext4_fs.cpp
        drivers/ext4_fs.h

        # lib
        lib/id_manager.h
        lib/lib_common.h
        lib/union_variant.h

        # app
        app/test.cpp
        src/hierarchy/content/ctl_dev/ctl_dev_pt.h
        src/hierarchy/content/ctl_dev/ctl_dev.h
        ctl_devs/proc/proc_ctl_dev.cpp
        ctl_devs/proc/proc_ctl_dev.h
        ctl_devs/sys/sys_ctl_dev.cpp
        ctl_devs/sys/sys_ctl_dev.h
        ctl_devs/hw/hw_ctl_dev.cpp
        ctl_devs/hw/hw_ctl_dev.h
        lib/num_wrapper.h
        src/hierarchy/content/stream_dev/stream_dev.h
        src/hierarchy/content/stream_dev/stream_dev_pt.h
        src/hierarchy/content/block_dev/block_dev.h
        stream_devs/random_stream_dev.cpp
        stream_devs/random_stream_dev.h

        src/hierarchy/comp/content/content_pt.h
        lib/avl_tree.h
        lib/queue.h
        lib/utility.h
        lib/utility.cpp
        lib/vector.h

)
