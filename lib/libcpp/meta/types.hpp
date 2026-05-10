#ifndef LIB_LIBCPP_META_TYPES_HPP_
#define LIB_LIBCPP_META_TYPES_HPP_

#include "libzig/meta/type_traits.hpp"
namespace libcpp::meta::types {
namespace reference {
template <class T>
struct RemoveReference final {
    using type = T;
};

template <class T>
struct RemoveReference<T&> final {
    using type = T;
};

template <class T>
struct RemoveReference<T&&> final {
    using type = T;
};

template <class T>
using remove_reference_t = typename RemoveReference<T>::type;

template <class T>
constexpr T&& forward(remove_reference_t<T>& any) noexcept {
    return static_cast<T&&>(any);
}

template <class T>
constexpr T&& forward(remove_reference_t<T>&& any) noexcept {
    return static_cast<T&&>(any);
}

template <class T>
constexpr remove_reference_t<T>&& move(T&& t) noexcept {
    return static_cast<remove_reference_t<T>&&>(t);
}
}  // namespace reference

namespace qualifiers {
template <class T>
struct RemoveConst final {
    using type = T;
};

template <class T>
struct RemoveConst<const T> final {
    using type = T;
};

template <class T>
struct RemoveVolatile final {
    using type = T;
};

template <class T>
struct RemoveVolatile<volatile T> final {
    using type = T;
};

template <class T>
struct RemoveCV final {
    using type = typename RemoveVolatile<typename RemoveConst<T>::type>::type;
};

static_assert(libzig::meta::cmp::same_type<RemoveCV<int>::type, int>);
static_assert(libzig::meta::cmp::same_type<RemoveCV<const int>::type, int>);
static_assert(libzig::meta::cmp::same_type<RemoveCV<volatile int>::type, int>);
static_assert(
    libzig::meta::cmp::same_type<RemoveCV<const volatile int>::type, int>);
}  // namespace qualifiers

namespace cmp {
template <bool Condition, class T, class F>
struct If final {
    static_assert(false);
};

template <class T, class F>
struct If<true, T, F> final {
    using type = T;
};

template <class T, class F>
struct If<false, T, F> final {
    using type = F;
};

// IfVal probably is more appropriately defined in type_traits.hpp, but I'll
// keep it here for proximity to If
template <bool Condition, auto Tv, auto Fv>
struct IfVal final {
    static_assert(false);
};

template <auto Tv, auto Fv>
struct IfVal<true, Tv, Fv> final {
    static inline constexpr decltype(Tv) value{Tv};
};

template <auto Tv, auto Fv>
struct IfVal<false, Tv, Fv> final {
    static inline constexpr decltype(Fv) value{Fv};
};
}  // namespace cmp

}  // namespace libcpp::meta::types
#endif  // LIB_LIBCPP_META_TYPES_HPP_
