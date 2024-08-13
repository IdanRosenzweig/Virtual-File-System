#ifndef COMP_ID_H
#define COMP_ID_H

#include "lib/num_wrapper.h"

type_define(comp_id_t, unsigned int)

inline comp_id_t comp_id_null(0);
inline comp_id_t comp_id_virt(1);

inline comp_id_t min_accessable_comp_id(2);

#endif //COMP_ID_H
