#ifndef NUM_WRAPPER_H
#define NUM_WRAPPER_H

#define FORCE_INLINE __attribute__((always_inline))

template<typename TYPE>
struct num_wrapper {
private:
    TYPE value;

public:
    // constructors
    FORCE_INLINE constexpr num_wrapper() noexcept : value(0) {
    }

    FORCE_INLINE constexpr num_wrapper(TYPE value) noexcept
        : value(value) {
    }

    FORCE_INLINE constexpr num_wrapper(const num_wrapper &other) noexcept
        : value(other.value) {
    }

    FORCE_INLINE constexpr num_wrapper(num_wrapper &&other) noexcept
        : value(other.value) {
    }

    // copy and move
    FORCE_INLINE constexpr num_wrapper &operator=(const num_wrapper &other) noexcept {
        value = other.value;
        return *this;
    }

    FORCE_INLINE constexpr num_wrapper &operator=(num_wrapper &&other) noexcept {
        value = other.value;
        return *this;
    }

    // casts
    FORCE_INLINE constexpr inline operator TYPE() noexcept {
        return value;
    }

    FORCE_INLINE constexpr inline operator TYPE &() noexcept {
        return value;
    }

    FORCE_INLINE constexpr inline operator const TYPE &() const noexcept {
        return value;
    }

    // arithmetic operators
    FORCE_INLINE constexpr inline num_wrapper operator+(const num_wrapper &other) const noexcept {
        return num_wrapper(value + other.value);
    }

    FORCE_INLINE constexpr inline num_wrapper operator-(const num_wrapper &other) const noexcept {
        return num_wrapper(value - other.value);
    }

    FORCE_INLINE constexpr inline num_wrapper operator*(const num_wrapper &other) const noexcept {
        return num_wrapper(value * other.value);
    }

    FORCE_INLINE constexpr inline num_wrapper operator/(const num_wrapper &other) const noexcept {
        return num_wrapper(value / other.value);
    }

    FORCE_INLINE constexpr inline num_wrapper operator%(const num_wrapper &other) const noexcept {
        return num_wrapper(value % other.value);
    }

    FORCE_INLINE constexpr inline num_wrapper &operator+=(const num_wrapper &other) noexcept {
        value += other.value;
        return *this;
    }

    FORCE_INLINE constexpr inline num_wrapper &operator-=(const num_wrapper &other) noexcept {
        value -= other.value;
        return *this;
    }

    FORCE_INLINE constexpr inline num_wrapper &operator*=(const num_wrapper &other) noexcept {
        value *= other.value;
        return *this;
    }

    FORCE_INLINE constexpr inline num_wrapper &operator/=(const num_wrapper &other) noexcept {
        value /= other.value;
        return *this;
    }

    FORCE_INLINE constexpr inline num_wrapper &operator%=(const num_wrapper &other) noexcept {
        value %= other.value;
        return *this;
    }

    // increment and decrement operators
    FORCE_INLINE constexpr inline num_wrapper &operator++() noexcept {
        ++value;
        return *this;
    }

    FORCE_INLINE constexpr inline num_wrapper operator++(int) noexcept {
        num_wrapper temp(*this);
        ++value;
        return temp;
    }

    FORCE_INLINE constexpr inline num_wrapper &operator--() noexcept {
        --value;
        return *this;
    }

    FORCE_INLINE constexpr inline num_wrapper operator--(int) noexcept {
        num_wrapper temp(*this);
        ++value;
        return temp;
    }

    // relational operators
    FORCE_INLINE constexpr inline bool operator==(const num_wrapper &other) const noexcept {
        return value == other.value;
    }

    FORCE_INLINE constexpr inline bool operator!=(const num_wrapper &other) const noexcept {
        return value != other.value;
    }

    FORCE_INLINE constexpr inline bool operator<(const num_wrapper &other) const noexcept {
        return value < other.value;
    }

    FORCE_INLINE constexpr inline bool operator<=(const num_wrapper &other) const noexcept {
        return value <= other.value;
    }

    FORCE_INLINE constexpr inline bool operator>(const num_wrapper &other) const noexcept {
        return value > other.value;
    }

    FORCE_INLINE constexpr inline bool operator>=(const num_wrapper &other) const noexcept {
        return value >= other.value;
    }
};

#define STRONG_TYPE_DEFINE 1

#if STRONG_TYPE_DEFINE == 1
#define type_define(name, type) \
struct name : public num_wrapper<type> { \
    using num_wrapper::num_wrapper; \
    using num_wrapper::operator=; \
    using num_wrapper::operator type; \
    using num_wrapper::operator type&; \
    using num_wrapper::operator const type&; \
    using num_wrapper::operator+; \
    using num_wrapper::operator-; \
    using num_wrapper::operator*; \
    using num_wrapper::operator/; \
    using num_wrapper::operator%; \
    using num_wrapper::operator+=; \
    using num_wrapper::operator-=; \
    using num_wrapper::operator*=; \
    using num_wrapper::operator/=; \
    using num_wrapper::operator%=; \
    using num_wrapper::operator++; \
    using num_wrapper::operator--; \
    using num_wrapper::operator==; \
    using num_wrapper::operator!=; \
    using num_wrapper::operator<; \
    using num_wrapper::operator<=; \
    using num_wrapper::operator>; \
    using num_wrapper::operator>=; \
};
#else
#define type_define(name, type) using name = type;
#endif

#endif //NUM_WRAPPER_H
