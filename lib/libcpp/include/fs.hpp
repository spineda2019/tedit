#ifndef SRC_INCLUDE_TYPES_HPP_
#define SRC_INCLUDE_TYPES_HPP_

#include <libzig/libzig.hpp>

#include "detail/fs.hpp"

namespace libzig::types {

inline file_t GetSpecialFileHandle(detail::SpecialFile sf) noexcept {
    return libzig::types::detail::GetSpecialFileHandleHelper<
        libzig::meta::comptime_values::platform_filehandle_type>(sf);
}

}  // namespace libzig::types

#endif  // SRC_INCLUDE_TYPES_HPP_
