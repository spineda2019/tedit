#ifndef LIB_LIBCPP_INCLUDE_STRINGVIEW_HPP_
#define LIB_LIBCPP_INCLUDE_STRINGVIEW_HPP_

#include "libcpp/meta/types.hpp"
#include "libzig/libzig.hpp"
namespace libcpp {
class StringView {
 public:
    StringView(char const* ptr, libzig::size_t len) : ptr_{ptr}, len_{len} {}

    template <class Self>
    auto&& CStr(this Self&& self) {
        return meta::types::reference::forward<Self>(self).ptr_;
    }

    template <class Self>
    auto&& Len(this Self&& self) {
        return meta::types::reference::forward<Self>(self).len_;
    }

 private:
    char const* ptr_;
    [[maybe_unused]]
    libzig::size_t len_;
};
}  // namespace libcpp

#endif  // LIB_LIBCPP_INCLUDE_STRINGVIEW_HPP_
