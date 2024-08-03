#ifndef LIB_COMMON_H
#define LIB_COMMON_H

/** helper for calculating some associate function on a pack of templates (compile time) */
template<typename OP, auto... VALS>
struct operation;

template<typename OP, auto VAL, auto... REST>
struct operation<OP, VAL, REST...> {
    constexpr static auto val = OP()(VAL, operation<OP, REST...>::val);
};

template<typename OP, auto VAL>
struct operation<OP, VAL> {
    constexpr static auto val = VAL;
};

/** sample function that can be used with "operation" */
struct max_op {
    template<typename T>
    constexpr T operator()(T a, T b) const {
        return (a > b) ? a : b;
    }
};

/** helper function to get the index of a type in a pack */
template<typename TYPE, typename... OPTIONS>
struct index_of_type;

template<typename TYPE>
struct index_of_type<TYPE> {
    static constexpr int index = -1;
};

template<typename TYPE, typename... OPTIONS>
struct index_of_type<TYPE, TYPE, OPTIONS...> {
    static constexpr int index = 0;
};

template<typename TYPE, typename SOME_TYPE, typename... OPTIONS>
struct index_of_type<TYPE, SOME_TYPE, OPTIONS...> {
    static constexpr int index = 1 + index_of_type<TYPE, OPTIONS...>::index;
};

/** helpers to get the i-th type from a variadic list */
template<int INDEX, typename First, typename... Rest>
    requires(0 <= INDEX)
struct type_at_index : type_at_index<INDEX - 1, Rest...> {
};

template<typename First, typename... Rest>
struct type_at_index<0, First, Rest...> {
    using type = First;
};

template<int INDEX, typename... TYPES>
    requires(0 <= INDEX)
using type_at_index_v = typename type_at_index<INDEX, TYPES...>::type;

// type traits
// template<class T, class U>
// struct is_same;
//
// template<class T, class U>
// struct is_same{
//     constexpr static auto val = false;
// };
//
// template<class T>
// struct is_same<T, T> {
//     constexpr static auto val = true;
// };

#endif //LIB_COMMON_H
