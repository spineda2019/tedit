// Copyright 2026 Sebastian Pineda

#ifndef SRC_LIBTEDIT_HPP_
#define SRC_LIBTEDIT_HPP_

namespace tedit {

extern "C" {
void enter_cooked_mode() noexcept;

void enter_raw_mode() noexcept;

unsigned char read_char() noexcept;

void print_char(unsigned char) noexcept;
}
}  // namespace tedit

#endif  // SRC_LIBTEDIT_HPP_
