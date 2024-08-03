#ifndef UNION_VARIANT_H
#define UNION_VARIANT_H

#include "lib_common.h"

/** aligned union-like storage for pack of types */
template<size_t MIN_SZ, typename... TYPES>
struct aligned_storaege {
    // static constexpr size_t alignment = std::max({alignof(TYPES)...});
    static constexpr size_t alignment = operation<max_op, alignof(TYPES)...>::val;
    // alignas(alignment) uint8_t data[std::max({MIN_SZ, sizeof(TYPES)...})];
    alignas(alignment) uint8_t data[operation<max_op, MIN_SZ, sizeof(TYPES)...>::val];
};

/** fundamental visitor instantiations */
struct delete_visitor {
    template<typename T>
    void operator()(T *ptr) { ptr->~T(); }

    void operator()(void) {
    }
};

struct copy_visitor {
    const void *src;

    template<typename TYPE>
    void operator()(TYPE *ptr) { new(ptr) TYPE(*static_cast<const TYPE *>(src)); }

    void operator()(void) {
    }
};

struct move_visitor {
    void *src;

    template<typename TYPE>
    void operator()(TYPE *ptr) { new(ptr) TYPE(std::move(*static_cast<TYPE *>(src))); }

    void operator()(void) {
    }
};

/** variant struct */
template<typename... ALTERNATIVES>
struct union_variant {
    bool empty = true;
    int curr_in = 0;
    aligned_storaege<0, ALTERNATIVES...> storage;

    union_variant() = default;

    template<typename TYPE>
        requires ( std::same_as<TYPE, ALTERNATIVES> || ... )
    union_variant(const TYPE &val) : empty(false), curr_in(index_of_type<TYPE, ALTERNATIVES...>::index) {
        new(reinterpret_cast<TYPE *>(storage.data)) TYPE(val);
    }

    template<typename TYPE>
        requires ( std::same_as<TYPE, ALTERNATIVES> || ... )
    union_variant(TYPE &&val) : empty(false), curr_in(index_of_type<TYPE, ALTERNATIVES...>::index) {
        new(reinterpret_cast<TYPE *>(storage.data)) TYPE(std::forward<TYPE>(val));
    }

    union_variant(const union_variant &other) : empty(other.empty), curr_in(other.curr_in) {
        visit(copy_visitor(reinterpret_cast<const void *>(&other.storage)));
    }

    union_variant &operator=(const union_variant &other) {
        if (this != &other) {
            this->~union_variant();
            new(this) union_variant(other);
        }
        return *this;
    }

    union_variant(union_variant &&other) noexcept : empty(other.empty), curr_in(other.curr_in) {
        visit(move_visitor(reinterpret_cast<void *>(&other.storage)));
    }

    union_variant &operator=(union_variant &&other) noexcept {
        if (this != &other) {
            this->~union_variant();
            new(this) union_variant(std::move(other));
        }
        return *this;
    }

    ~union_variant() {
        empty = true;
        visit(delete_visitor());
    }

private:
    template<unsigned CURR_IN, typename TYPE, typename... REST>
        requires(0 <= CURR_IN)
    struct visit_rec_struct {
        template<class VISITOR>
        static auto visit(int index, void *data, VISITOR &&vis) {
            return CURR_IN == index
                       ? vis(reinterpret_cast<TYPE *>(data))
                       : visit_rec_struct<CURR_IN + 1, REST...>::visit(index, data, std::forward<VISITOR>(vis));
        }
    };

    template<unsigned CURR_IN, typename TYPE>
        requires(0 <= CURR_IN)
    struct visit_rec_struct<CURR_IN, TYPE> {
        template<typename VISITOR>
        static auto visit(int index, void *data, VISITOR &&vis) {
            if (CURR_IN == index) return vis(reinterpret_cast<TYPE *>(data));
            vis();
        }
    };

public:
    template<class VISITOR>
    auto visit(VISITOR &&vis) {
        return visit_rec_struct<0, ALTERNATIVES...>::visit(curr_in, storage.data, std::forward<VISITOR>(vis));
    }

    inline bool is_empty() noexcept {
        return empty;
    }

    inline int curr_index() const noexcept {
        return curr_in;
    }

    template<int INDEX>
        requires(INDEX < sizeof...(ALTERNATIVES))
    auto *get_by_index() {
        if (empty || curr_in != INDEX) throw;
        return reinterpret_cast<type_at_index<INDEX, ALTERNATIVES...> *>(storage.data);
    }

    template<typename TYPE>
        requires ( std::same_as<TYPE, ALTERNATIVES> || ... )
    auto *get_by_type() {
        if (empty || curr_in != index_of_type<TYPE, ALTERNATIVES...>::index) throw;
        return reinterpret_cast<TYPE *>(storage.data);
    }

    template<typename TYPE, typename... ARGS>
        requires ( std::same_as<TYPE, ALTERNATIVES> || ... )
    void set(ARGS &&... args) {
        empty = false;
        curr_in = index_of_type<TYPE, ALTERNATIVES...>::index;
        new(reinterpret_cast<TYPE *>(storage.data)) TYPE(std::forward<ARGS>(args)...);
    }

    template<typename TYPE>
        requires ( std::same_as<TYPE, ALTERNATIVES> || ... )
    static constexpr int type_index() noexcept {
        return index_of_type<TYPE, ALTERNATIVES...>::index;
    }
};

#endif //UNION_VARIANT_H
