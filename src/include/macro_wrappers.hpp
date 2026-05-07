/// Copyright 2026 Sebastian Pineda
/// \file macro_wrappers.hpp
/// C macros such and we prefer constexpr stuff where we can.
///
/// All macros that _can_ be represented with a stronger type _should_ be
/// wrapped up here, especially for use with any template meta programming.

#ifndef SRC_INCLUDE_MACRO_WRAPPERS_HPP_
#define SRC_INCLUDE_MACRO_WRAPPERS_HPP_

#include "./meta.hpp"

namespace tedit::meta {
inline static constexpr tedit::meta::fs::FileHandleType
    platform_filehandle_type{
#ifdef TEDIT_POSIX
        tedit::meta::fs::FileHandleType::FileDescriptor
#elifdef TEDIT_WIN32
        tedit::meta::fs::FileHandleType::Handle
#else
#error "Could not detect target platform (needed for filehandle types)"
#endif
    };

inline static constexpr tedit::meta::platform::Os target_platform {
#ifdef TEDIT_POSIX
    // TODO(SEP): somehow detect mac
    tedit::meta::platform::Os::Linux
#elifdef TEDIT_WIN32
    tedit::meta::platform::Os::Windows;
#else
#error "Could not detect target platform"
#endif
};

namespace detail {
inline consteval bool EnsurePlatformCorrectness() {
    bool result{true};

    switch (target_platform) {
        case platform::Os::Linux:
        case platform::Os::Mac:
            result =
                platform_filehandle_type == fs::FileHandleType::FileDescriptor;
            break;
        case platform::Os::Windows:
            result = platform_filehandle_type == fs::FileHandleType::Handle;
            break;
    }

    return result;
}
}  // namespace detail

static_assert(detail::EnsurePlatformCorrectness(), "Oops");

}  // namespace tedit::meta

#endif  // SRC_INCLUDE_MACRO_WRAPPERS_HPP_
