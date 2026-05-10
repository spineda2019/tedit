#ifndef LIB_LIBCPP_META_TYPES_HPP_
#define LIB_LIBCPP_META_TYPES_HPP_

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

}  // namespace cmp

namespace signedness {
// TODO(SEP): IsSigned meta functions
}  // namespace signedness

}  // namespace libcpp::meta::types
#endif  // LIB_LIBCPP_META_TYPES_HPP_
