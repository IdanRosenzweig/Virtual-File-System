#ifndef CTX_T_H
#define CTX_T_H

struct hierarchy;

template<typename T>
struct ctx_t {
    hierarchy *hier = nullptr;
    T val;

    ctx_t() {
    }

    ctx_t(hierarchy *hierarchy, const T &val) noexcept
        : hier(hierarchy),
          val(val) {
    }

    ctx_t(hierarchy *hierarchy, T &&val) noexcept
        : hier(hierarchy),
          val(std::move(val)) {
    }

    ctx_t(const ctx_t &other)
        : hier(other.hier),
          val(other.val) {
    }

    ctx_t(ctx_t &&other) noexcept
        : hier(other.hier),
          val(std::move(other.val)) {
    }

    ctx_t &operator=(const ctx_t &other) {
        if (this == &other)
            return *this;
        hier = other.hier;
        val = other.val;
        return *this;
    }

    ctx_t &operator=(ctx_t &&other) noexcept {
        if (this == &other)
            return *this;
        hier = other.hier;
        val = std::move(other.val);
        return *this;
    }

    friend bool operator==(const ctx_t &lhs, const ctx_t &rhs) {
        return lhs.hier == rhs.hier
               && lhs.val == rhs.val;
    }

    friend bool operator!=(const ctx_t &lhs, const ctx_t &rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const ctx_t &lhs, const ctx_t &rhs) {
        if (lhs.hier != rhs.hier) return lhs.hier < rhs.hier;
        return lhs.val < rhs.val;
    }

    friend bool operator<=(const ctx_t &lhs, const ctx_t &rhs) {
        return !(rhs < lhs);
    }

    friend bool operator>(const ctx_t &lhs, const ctx_t &rhs) {
        return rhs < lhs;
    }

    friend bool operator>=(const ctx_t &lhs, const ctx_t &rhs) {
        return !(lhs < rhs);
    }
};

// wrappers for creating null ctx
template<typename T>
ctx_t<T> null_ctx() { return ctx_t<T>(); }

template<typename T>
ctx_t<T> null_ctx(const T &val) { return ctx_t<T>(nullptr, val); }

template<typename T>
ctx_t<T> null_ctx(T &&val) { return ctx_t<T>(nullptr, std::move(val)); }

// wrapper for checking null ctx
template<typename T>
inline bool is_ctx_null(const ctx_t<T> &ctx) noexcept {
    return ctx.hier == nullptr;
}

#endif //CTX_T_H
