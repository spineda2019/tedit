/// Copyright 2026 Sebastian Pineda
/// \file CStringView.hpp

#ifndef SRC_INCLUDE_CSTRINGVIEW_HPP_
#define SRC_INCLUDE_CSTRINGVIEW_HPP_

#include "./types.hpp"

namespace tedit {
class CStringView final {
 public:
    explicit CStringView() noexcept;
    explicit CStringView(unsigned char const*, tedit::types::size_t) noexcept;

 private:
    unsigned char const* data_;
    tedit::types::size_t len_;
};
}  // namespace tedit

#endif  // SRC_INCLUDE_CSTRINGVIEW_HPP_
