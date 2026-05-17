#ifndef LIB_LIBCPP_META_TYPE_TRAITS_HPP_
#define LIB_LIBCPP_META_TYPE_TRAITS_HPP_

#include <libzig/meta/type_traits.hpp>

namespace libcpp::meta::types {

template <class T>
struct IsPointer final {
    inline static constexpr bool value{false};
};

template <class T>
struct IsPointer<T*> final {
    inline static constexpr bool value{true};
};

template <class T>
struct IsPointer<T const*> final {
    inline static constexpr bool value{true};
};

namespace numeric {

template <class T>
struct IsIntegral final {
    inline static constexpr bool value{requires(T t, T* ptr, void (*func)(T)) {
        func(0);  // we're stuck with weird C-style function-pointer syntax.
                  // Here, func is just a function that returns nothing (void)
                  // and takes in a T as an argument (which we're trying to
                  // ensure is integer like)
        reinterpret_cast<T>(t);  // supposedly this excludes custom class types
                                 // (according to cppreference.com)
        T{1};
        ptr + t;
    }};
};

template <class T>
struct IsFloating final {
    inline static constexpr bool value{
        libzig::meta::cmp::OneOf<T, float, double, long double>::value};
};

template <class T>
struct IsArithmetic final {
    inline static constexpr bool value{IsIntegral<T>::value ||
                                       IsFloating<T>::value};
};

template <class T, bool b = IsArithmetic<T>::value>
struct IsSigned final {
    inline static constexpr bool value{T{-1} < T{0}};
};

template <class T>
struct IsSigned<T, false> final {
    inline static constexpr bool value{false};
};

}  // namespace numeric
}  // namespace libcpp::meta::types

#endif  // LIB_LIBCPP_META_TYPE_TRAITS_HPP_
