#ifndef LIB_LIBCPP_INCLUDE_ARRAY_HPP_
#define LIB_LIBCPP_INCLUDE_ARRAY_HPP_

#include <libzig/libzig.hpp>

namespace libcpp {

template <class T, libzig::size_t N>
struct Array final {
    T data_[N];

    using iterator = T*;
    using const_iterator = T const*;
    using value_type = T;
    using reference = T&;
    using pointer = T*;

    template <class Self>
    constexpr decltype(auto) operator[](this Self&& self, libzig::size_t i) {
        return self.data_[i];
    }

    static constexpr libzig::size_t Size() { return N; }

    constexpr libzig::size_t len() const { return N; }

    constexpr auto begin(this auto&& self) { return self.data_; }

    constexpr T const* cbegin(this auto&& self) { return self.data_; }

    constexpr auto end(this auto&& self) { return self.data_ + N; }

    constexpr T const* cend(this auto&& self) { return self.data_ + N; }
};
}  // namespace libcpp

#endif  // LIB_LIBCPP_INCLUDE_ARRAY_HPP_
