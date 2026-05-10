#ifndef LIB_LIBCPP_INCLUDE_IO_TERMINAL_HPP_
#define LIB_LIBCPP_INCLUDE_IO_TERMINAL_HPP_

namespace libcpp::io {
namespace terminal {
void EnterRawMode() noexcept;
void EnterCookedMode() noexcept;
}  // namespace terminal
}  // namespace libcpp::io

#endif  // LIB_LIBCPP_INCLUDE_IO_TERMINAL_HPP_
