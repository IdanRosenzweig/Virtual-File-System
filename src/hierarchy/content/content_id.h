#ifndef CONTENT_ID_H
#define CONTENT_ID_H

#include "lib/num_wrapper.h"

type_define(content_id_t, unsigned int)

inline content_id_t content_id_null(0);
inline content_id_t min_accessable_content_id(1);

#endif //CONTENT_ID_H
