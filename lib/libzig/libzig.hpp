// Copyright 2026 Sebastian Pineda

#ifndef SRC_INCLUDE_LIBTEDIT_HPP_
#define SRC_INCLUDE_LIBTEDIT_HPP_

#include "meta/comptime_values.hpp"
#include "meta/types.hpp"

// this must use the C ABI not because we support C consumers, but because that
// is the only stable ABI zig can use to export functions
extern "C" {
namespace libzig {
using size_t = typename meta::types::core::SizeT::type;
using file_t = typename meta::types::fs::FileHandle<
    meta::comptime_values::platform_filehandle_type>::type;
static_assert(sizeof(file_t) > 1, "sizeof file_t type must be > 1");

void enter_cooked_mode() noexcept;

void enter_raw_mode() noexcept;

unsigned char read_char(file_t) noexcept;

void write_char(file_t, unsigned char) noexcept;

void clear_screen() noexcept;

file_t open(const char*) noexcept;

file_t open_stdout() noexcept;

file_t open_stdin() noexcept;

file_t open_stderr() noexcept;
}  // namespace libzig

}  // extern "C"

#endif  // SRC_INCLUDE_LIBTEDIT_HPP_
