// Copyright 2026 Sebastian Pineda

#ifndef SRC_INCLUDE_LIBTEDIT_HPP_
#define SRC_INCLUDE_LIBTEDIT_HPP_

#include "./macro_wrappers.hpp"
#include "./meta.hpp"

namespace tedit {

extern "C" {
void enter_cooked_mode() noexcept;

void enter_raw_mode() noexcept;

unsigned char read_char() noexcept;

void print_char(unsigned char) noexcept;

types::file_handle_t clear_screen() noexcept;
}
}  // namespace tedit

#endif  // SRC_INCLUDE_LIBTEDIT_HPP_
