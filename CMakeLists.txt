cmake_minimum_required(VERSION 3.28)
project(src)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(vfs
        # base
        src/hierarchy/base/node/node.cpp
        src/hierarchy/base/node/node_name.h
        src/hierarchy/base/node/node_id.h

        src/hierarchy/base/path/path.h
        src/hierarchy/base/path/path.cpp


        # data
        src/hierarchy/data/node_data.h
        src/hierarchy/data/node_data_id.h

        src/hierarchy/data/null_data/null_data.h

        src/hierarchy/data/dir/dir.h
        src/hierarchy/data/dir/dir.cpp

        src/hierarchy/data/soft_link/soft_link.h

        src/hierarchy/data/object/object.h
        src/hierarchy/data/object/obj_data.h

        src/hierarchy/data/object/text_file/text_file.cpp
        src/hierarchy/data/object/text_file/text_file.h


        # fs
        src/fs/fs.h
        src/fs/fs.cpp
        src/fs/fs_node_ops.h
        src/fs/fs_ops.h
        src/fs/loaded_node.h

        lib/id_manager.h
        lib/lib_common.h

        samples/ramfs.cpp
        samples/ramfs.h

        samples/ext4_fs.cpp
        samples/ext4_fs.h

        main/test.cpp



)
