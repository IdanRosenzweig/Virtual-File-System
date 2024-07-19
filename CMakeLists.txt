cmake_minimum_required(VERSION 3.28)
project(src)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(vfs
        # hierarchy
        src/hierarchy/basic_hierarchy.h
        src/hierarchy/basic_hierarchy.cpp
        src/hierarchy/basic_driver.h

        # node
        src/hierarchy/node/node_name.h
        src/hierarchy/node/node_id.h

        # path
        src/hierarchy/path/path.h
        src/hierarchy/path/path.cpp

        # comps
        src/hierarchy/comp/comp.h
        src/hierarchy/comp/comp_id.h
        src/hierarchy/comp/base_comp.h

        src/hierarchy/comp/dir/dir.h
        src/hierarchy/comp/dir/dir.cpp

        src/hierarchy/comp/softlink/softlink.h
        src/hierarchy/comp/softlink/soft_link.cpp

        # refs
        src/hierarchy/refs/refs.h
        src/hierarchy/refs/refs_id.h

        # content
        src/hierarchy/content/content.h
        src/hierarchy/content/content_id.h
        src/hierarchy/content/base_content.h

        src/hierarchy/comp/null_comp/null_comp.h

        src/hierarchy/content/textfile/textfile.cpp
        src/hierarchy/content/textfile/textfile.h

        src/hierarchy/content/mount/mount.h
        src/hierarchy/content/mount/mount.h

        # content fs
        src/content_fs/content_fs.cpp
        src/content_fs/content_fs.h
        src/content_fs/fs_id.h

        # mounter
        src/mounter/mounter.cpp
        src/mounter/mounter.h

        # drivers
        src/drivers/ram_driver.cpp
        src/drivers/ram_driver.h

        src/drivers/ext4_fs.cpp
        src/drivers/ext4_fs.h

        # lib
        lib/id_manager.h
        lib/lib_common.h

        # main
        src/main/test.cpp
        src/hierarchy/content/null_content/null_content.h

)
