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

}  // namespace tedit::meta

#endif  // SRC_INCLUDE_MACRO_WRAPPERS_HPP_
