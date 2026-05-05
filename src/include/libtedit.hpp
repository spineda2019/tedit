// Copyright 2026 Sebastian Pineda

#ifndef SRC_INCLUDE_LIBTEDIT_HPP_
#define SRC_INCLUDE_LIBTEDIT_HPP_

#include "./types.hpp"

namespace tedit {

extern "C" {
void enter_cooked_mode() noexcept;

void enter_raw_mode() noexcept;

unsigned char read_char() noexcept;

void write_char(types::file_handle_t, unsigned char) noexcept;

void clear_screen() noexcept;

tedit::types::file_handle_t open(const char*) noexcept;
}
}  // namespace tedit

#endif  // SRC_INCLUDE_LIBTEDIT_HPP_
