#include "lib_common.h"

#include <stdlib.h>

void *my_malloc(int sz) {
    return malloc(sz);
}

void my_free(void *ptr) {
    free(ptr);
}

void *operator new(size_t sz) {
    if (sz == 0) sz = 1;
    return my_malloc(sz);
}

void *operator new [](size_t sz) {
    if (sz == 0) sz = 1;
    return my_malloc(sz);
}

void *operator new(size_t sz, void *ptr) {
    return ptr;
}

void *operator new [](size_t sz, void *ptr) {
    return ptr;
}

void operator delete(void *ptr) noexcept {
    my_free(ptr);
}

void operator delete[](void *ptr) noexcept {
    my_free(ptr);
}
