#ifndef SRC_INCLUDE_TYPES_HPP_
#define SRC_INCLUDE_TYPES_HPP_

#include "./macro_wrappers.hpp"
#include "./meta.hpp"

namespace tedit::types {
using size_t = tedit::meta::size_type::type;

using file_handle_t = typename tedit::meta::FileHandle<
    tedit::meta::platform_filehandle_type>::type;

static_assert(sizeof(file_handle_t) > 1, "sizeof File Handle type must be > 1");
}  // namespace tedit::types

#endif  // SRC_INCLUDE_TYPES_HPP_
