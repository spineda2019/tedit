#ifndef LIB_LIBZIG_META_COMPTIME_VALUES_HPP_
#define LIB_LIBZIG_META_COMPTIME_VALUES_HPP_

#include "./types.hpp"

namespace libzig::meta::comptime_values {
inline static constexpr libzig::meta::tags::fs::FileHandleTag
    platform_filehandle_type{
#ifdef TEDIT_POSIX
        libzig::meta::tags::fs::FileHandleTag::FileDescriptor
#elifdef TEDIT_WIN32
        libzig::meta::tags::fs::FileHandleTag::Handle
#else
#error "Could not detect target platform (needed for filehandle types)"
#endif
    };

inline static constexpr libzig::meta::tags::platform::Os target_platform {
#ifdef TEDIT_POSIX
    // TODO(SEP): somehow detect mac
    libzig::meta::tags::platform::Os::Linux
#elifdef TEDIT_WIN32
    libzig::meta::tags::platform::Os::Windows;
#else
#error "Could not detect target platform"
#endif
};

namespace detail {
inline consteval bool EnsurePlatformCorrectness() {
    bool result{true};

    switch (target_platform) {
        case libzig::meta::tags::platform::Os::Linux:
        case libzig::meta::tags::platform::Os::Mac:
            result = platform_filehandle_type ==
                     libzig::meta::tags::fs::FileHandleTag::FileDescriptor;
            break;
        case libzig::meta::tags::platform::Os::Windows:
            result = platform_filehandle_type ==
                     libzig::meta::tags::fs::FileHandleTag::Handle;
            break;
    }

    return result;
}
}  // namespace detail

static_assert(detail::EnsurePlatformCorrectness(), "Oops");

}  // namespace libzig::meta::comptime_values

#endif  // LIB_LIBZIG_META_COMPTIME_VALUES_HPP_
