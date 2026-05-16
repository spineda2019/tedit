#ifndef LIB_LIBCPP_INCLUDE_ARRAY_HPP_
#define LIB_LIBCPP_INCLUDE_ARRAY_HPP_

#include <libzig/libzig.hpp>

template <class T, libzig::size_t N>
struct Array final {
    T data_[N];

    template <class Self>
    constexpr decltype(auto) operator[](this Self&& self, libzig::size_t i) {
        return self.data_[i];
    }

    constexpr libzig::size_t Size() const { return N; }

    constexpr auto begin(this auto&& self) { return self.data_; }

    constexpr auto end(this auto&& self) { return self.data_ + N; }
};

#endif  // LIB_LIBCPP_INCLUDE_ARRAY_HPP_
