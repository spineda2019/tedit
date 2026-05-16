#ifndef LIB_LIBCPP_INCLUDE_UTILITY_HPP_
#define LIB_LIBCPP_INCLUDE_UTILITY_HPP_

#include "libcpp/meta/types.hpp"
namespace libcpp {
template <class T, class U = T>
constexpr  // Since C++20
    T exchange(T& obj, U&& new_value) noexcept {
    T old_value = meta::types::reference::move(obj);
    obj = meta::types::reference::forward<U>(new_value);
    return old_value;
}
}  // namespace libcpp

#endif  // LIB_LIBCPP_INCLUDE_UTILITY_HPP_
