// Copyright 2026 Sebastian Pineda

#ifndef SRC_INCLUDE_LIBTEDIT_HPP_
#define SRC_INCLUDE_LIBTEDIT_HPP_

#include "./meta.hpp"

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

namespace tedit {

using file_handle_t =
    typename tedit::meta::FileHandle<platform_filehandle_type>::type;

static_assert(sizeof(file_handle_t) > 1, "sizeof File Handle type must be > 1");

extern "C" {
void enter_cooked_mode() noexcept;

void enter_raw_mode() noexcept;

unsigned char read_char() noexcept;

void print_char(unsigned char) noexcept;

file_handle_t clear_screen() noexcept;
}
}  // namespace tedit

#endif  // SRC_INCLUDE_LIBTEDIT_HPP_
