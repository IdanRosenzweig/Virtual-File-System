#ifndef LIB_COMMON_H
#define LIB_COMMON_H

#include <cstddef>
#include <variant>

/** helpers to case vodi pointer */
#define cast_ptr(void_ptr, type_ptr) (reinterpret_cast<type_ptr*>(void_ptr))
#define cast_const_ptr(void_ptr, type_ptr) (reinterpret_cast<const type_ptr*>(void_ptr))

/** helpers to get the i-th type from a variadic list */
// template to get the i-th type from a variadic template
template<size_t INDEX, typename First, typename... Rest>
struct type_at_index : type_at_index<INDEX - 1, Rest...> {
};

// Specialization for the base case when the index is 0
template<typename First, typename... Rest>
struct type_at_index<0, First, Rest...> {
    using type = First;
};

// wrapper function
template<size_t INDEX, typename... TYPES>
using type_at_index_v = typename type_at_index<INDEX, TYPES...>::type;

/** helpers to get the index of a type in a variadic list */
// Helper template to find the index of a type in a type list
template<typename T, typename... Ts>
struct find_type_index;

// Base case: Type not found
template<typename T>
struct find_type_index<T> {
    static constexpr int index = -1;
};

// Recursive case: Type found at current index
template<typename T, typename... Ts>
struct find_type_index<T, T, Ts...> {
    static constexpr int index = 0;
};

// Recursive case: Type not found at current index, continue searching
template<typename T, typename U, typename... Ts>
struct find_type_index<T, U, Ts...> {
    static constexpr int index = 1 + find_type_index<T, Ts...>::index;

};

/** helper function to find the index of a type in a variant */
template<typename T, typename Variant>
int variant_index;

template<typename T, typename... Ts>
constexpr int variant_index<T, std::variant<Ts...>> =  find_type_index<T, Ts...>::index;


#endif //LIB_COMMON_H
