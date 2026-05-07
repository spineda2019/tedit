#ifndef SRC_INCLUDE_ARRAY_HPP_
#define SRC_INCLUDE_ARRAY_HPP_

#include "./types.hpp"

template <class T, tedit::types::size_t N>
struct Array final {
    T data_[N];

    template <class Self>
    constexpr T& operator[](this Self&& self, tedit::types::size_t i) {
        return self.data_[i];
    }

    constexpr tedit::types::size_t Size() const { return N; }

    template <class Self>
    constexpr auto begin(this Self&& self) {
        return self.data_;
    }

    template <class Self>
    constexpr auto end(this Self&& self) {
        return self.data_ + N;
    }
};

#endif  // SRC_INCLUDE_ARRAY_HPP_
