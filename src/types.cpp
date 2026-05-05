#include "include/types.hpp"

#include "include/macro_wrappers.hpp"
#include "include/meta.hpp"

namespace tedit::types {
file_handle_t GetSpecialFileHandle(tedit::meta::fs::SpecialFile sf) noexcept {
    return detail::GetSpecialFileHandleHelper<
        tedit::meta::platform_filehandle_type>(sf);
}
}  // namespace tedit::types
