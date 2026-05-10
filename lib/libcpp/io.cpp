#include "include/io.hpp"

#include <libzig/libzig.hpp>

namespace libcpp::io {
namespace terminal {
void EnterRawMode() noexcept { libzig::enter_raw_mode(); }
void EnterCookedMode() noexcept { libzig::enter_cooked_mode(); }
}  // namespace terminal
}  // namespace libcpp::io
