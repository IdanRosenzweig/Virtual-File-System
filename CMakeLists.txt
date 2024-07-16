cmake_minimum_required(VERSION 3.28)
project(src)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(vfs
        src/hierarchy/node/node.cpp
        src/hierarchy/node/node_name.h
        src/hierarchy/node/node_id.h
        src/hierarchy/node/node_data.h

        src/hierarchy/path/path.h

        src/hierarchy/null_node/null_node.h

        src/hierarchy/object/object.h

        src/hierarchy/dir/dir.h
        src/hierarchy/dir/dir.cpp

        src/hierarchy/soft_link/soft_link.h

        src/fs/fs.h
        src/fs/fs.cpp
        src/fs/fs_node_ops.h
        src/fs/fs_ops.h

        lib/id_manager.h
        lib/lib_common.h

        samples/ramfs.cpp
        samples/ramfs.h

        samples/ext4_fs.cpp
        samples/ext4_fs.h

        main/test.cpp
        src/hierarchy/text_file/text_file.cpp
        src/hierarchy/text_file/text_file.h
        src/hierarchy/path/path.cpp
        src/hierarchy/object/obj_data.h


)
