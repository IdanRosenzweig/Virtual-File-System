#ifndef VECTOR_H
#define VECTOR_H

#include "utility.h"

template<typename TYPE>
struct vector {
private:
    static constexpr double MULTIPLIER = 1.5;

    byte *alloc = nullptr; // allocation of bytes
    int alloc_sz = 0; // allocation size in terms of elements

    int sz = 0; // size in terms of elements

    void reserve(int no_items) {
        if (no_items <= alloc_sz) return;

        // allocate space
        byte *new_alloc = new byte[no_items * sizeof(TYPE)];

        // move the data
        for (int i = 0; i < sz * sizeof(TYPE); i++) {
            new_alloc[i] = alloc[i];
            alloc[i] = 0;
        }

        if (alloc != nullptr) delete alloc;
        alloc = new_alloc;

        alloc_sz = no_items;
    }

public:
    vector() = default;

    explicit vector(const TYPE &val) {
        push_back(val);
    }

    vector(const vector &other) {
        reserve(other.alloc_sz);
        sz = other.sz;
        for (int i = 0; i < sz * sizeof(TYPE); i++)
             alloc[i] = other.alloc[i];
    }

    vector(vector &&other) noexcept : alloc(other.alloc), alloc_sz(other.alloc_sz), sz(other.sz) {
        other.alloc = nullptr;
    }

    vector &operator=(const vector &other) {
        reserve(other.alloc_sz);
        sz = other.sz;
        for (int i = 0; i < sz * sizeof(TYPE); i++)
            alloc[i] = other.alloc[i];

        return *this;
    }

    vector &operator=(vector &&other) noexcept {
        alloc = other.alloc;
        alloc_sz = other.alloc_sz;
        sz = other.sz;
        other.alloc = nullptr;

        return *this;
    }

    ~vector() {
        // if (alloc != nullptr)
        // delete alloc;
    }

    void push_back(const TYPE &val) noexcept {
        if (!(sz < alloc_sz)) {
            int new_alloc_sz = alloc_sz * MULTIPLIER;
            if (new_alloc_sz == alloc_sz) ++new_alloc_sz;

            reserve(new_alloc_sz);
        }

        new (((TYPE*) alloc) + (sz++)) TYPE(val);
        // ((TYPE*) alloc)[sz++] = val;
    }

    void pop_back() noexcept {
        if (sz == 0) return;
        --sz;
    }

    TYPE &back() noexcept { return this->operator[](sz - 1); }
    const TYPE &back() const noexcept { return this->operator[](sz - 1); }

    TYPE &operator[](int in) noexcept {
        return ((TYPE*) alloc)[in];
    }

    const TYPE &operator[](int in) const noexcept {
        return ((TYPE*) alloc)[in];
    }

    TYPE *data() noexcept { return (TYPE*) alloc; }
    const TYPE *data() const noexcept { return (const TYPE*) alloc; }

    void clear() noexcept {
        if (alloc != nullptr) {
            delete alloc;
            alloc = nullptr;
        }
        alloc_sz = sz = 0;
    }

    int size() const noexcept { return sz; }

    bool empty() const noexcept { return sz == 0; }
};

#endif //VECTOR_H
