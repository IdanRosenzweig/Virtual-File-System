#ifndef OBJ_DATA_H
#define OBJ_DATA_H

#include <variant>
#include "../text_file/text_file.h"

enum class obj_data_type {
    TEXT_FILE
};
using obj_data = std::variant<text_file>;

#endif //OBJ_DATA_H
