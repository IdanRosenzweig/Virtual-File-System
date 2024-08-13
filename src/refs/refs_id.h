#ifndef REFS_ID_H
#define REFS_ID_H

#include "lib/num_wrapper.h"

type_define(refs_id_t, unsigned int)

inline refs_id_t refs_id_null(0);
inline refs_id_t min_accessable_refs_id(1);

#endif //REFS_ID_H
