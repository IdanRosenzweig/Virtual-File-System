#ifndef UTILITY_H
#define UTILITY_H

using byte = char;

void memcpy(void *dest, const void *src, int cnt);

void memset(void *dest, byte val, int cnt);

void strcpy(void *dest, const void *src);

template<class T>
struct rm_ref {
    typedef T type;
};

template<class T>
struct rm_ref<T &> {
    typedef T type;
};

template<class T>
struct rm_ref<T &&> {
    typedef T type;
};

template<class T>
struct is_lvalue_ref {
    constexpr static bool val = false;
};

template<class T>
struct is_lvalue_ref<T &> {
    constexpr static bool val = true;
};

template<typename T>
constexpr typename rm_ref<T>::type &&move(T &&t) noexcept {
    return static_cast<typename rm_ref<T>::type &&>(t);
}

template<typename T>
constexpr T &&forward(typename rm_ref<T>::type &t) noexcept {
    return static_cast<T &&>(t);
}

template<typename T>
constexpr T &&forward(typename rm_ref<T>::type &&t) noexcept {
    static_assert(!is_lvalue_ref<T>::val);
    return static_cast<T &&>(t);
}

#endif //UTILITY_H
