cmake_minimum_required(VERSION 3.28)
project(src)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(vfs
        # hierarchy
        src/hierarchy/node/node.cpp
        src/hierarchy/node/node_name.h
        src/hierarchy/node/node_id.h

        src/hierarchy/path/path.h
        src/hierarchy/path/path.cpp

        src/hierarchy/refs/node_refs.h
        src/hierarchy/refs/node_refs_id.h

        # data
        src/hierarchy/data/node_data.h
        src/hierarchy/data/node_data_id.h
        src/hierarchy/data/base_data.h

        src/hierarchy/data/null_data/null_data.h

        src/hierarchy/data/dir/dir.h
        src/hierarchy/data/dir/dir.cpp

        src/hierarchy/data/soft_link/soft_link.h
        src/hierarchy/data/soft_link/soft_link.cpp

        src/hierarchy/data/text_file/text_file.cpp
        src/hierarchy/data/text_file/text_file.h

        src/hierarchy/data/mount/mount.h
        src/hierarchy/data/mount/mount.h

        # fs
        src/fs/fs.h
        src/fs/fs.cpp
        src/fs/loaded_node.h

        lib/id_manager.h
        lib/lib_common.h

        samples/ramfs.cpp
        samples/ramfs.h

        samples/ext4_fs.cpp
        samples/ext4_fs.h

        main/test.cpp
        src/fs/fs_id.h

)
