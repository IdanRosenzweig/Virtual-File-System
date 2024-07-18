#include "soft_link.h"

void soft_link::set_soft_link_path(soft_link *link, const path &path) {
    link->path_ptr = path;
}
